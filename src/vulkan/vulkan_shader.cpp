#include "vulkan_shader.hpp"

#include "vulkan_check.hpp"
#include "vulkan_descriptor_pool.hpp"
#include "vulkan_descriptor_set_layout.hpp"
#include "vulkan_pipeline_layout.hpp"
#include "vulkan_shader_module.hpp"
#include "vulkan_vertex_descriptor.hpp"
#include "vulkan_window.hpp"

namespace cl::Vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

VulkanShader::VulkanShader(VulkanContextData* context, const ShaderCreateInfo& shader_info, bool enable_depth_test,
    bool enable_backface_cull, WindingOrder front_face, VkExtent2D render_target_extent, VkRenderPass render_pass)
    : context_(context), enable_depth_test_(enable_depth_test), enable_backface_cull_(enable_backface_cull), front_face_(front_face) {

  ShaderCodeMap code_map = ReadAllSpvFiles(shader_info);
  for (auto& code : code_map) {
    shader_modules_.emplace(FindVulkanShaderStage(code.first), CreateShaderModule(context, code.second));
  }

  reflection_details_.Reflect(code_map);
  // We only need descriptors if the shader actually contains any uniform buffers or texture samplers
  if (reflection_details_.HasUniformsOrTextures()) {
    descriptor_set_layout_ = CreateDescriptorSetLayout(context, reflection_details_);
    for (const auto& uniform : reflection_details_.uniforms) {
      uniform_buffers_.emplace(uniform.first, std::make_unique<VulkanUniformBuffer>(context, uniform.second));
    }
    for (const auto& sampler : reflection_details_.textures) {
      texture_samplers_.emplace(sampler.first, std::make_unique<VulkanTextureSampler>());
    }
  }

  graphics_pipeline_layout_ = CreatePipelineLayout(context, descriptor_set_layout_);
  CreatePipeline(render_target_extent, render_pass);

  if (reflection_details_.HasUniformsOrTextures()) {
    descriptor_pool_ = CreateDescriptorPool(context, reflection_details_);
    descriptor_sets_ = AllocateDescriptorSets(context, uniform_buffers_, texture_samplers_, descriptor_set_layout_, descriptor_pool_);
  }
}

VulkanShader::~VulkanShader() {
  vkDeviceWaitIdle(context_->device);

  if (reflection_details_.HasUniformsOrTextures()) {
    vkDestroyDescriptorPool(context_->device, descriptor_pool_, context_->allocator);
    uniform_buffers_.clear();
    // TODO: Destroy samplers
  }

  vkDestroyPipeline(context_->device, graphics_pipeline_, context_->allocator);

  vkDestroyPipelineLayout(context_->device, graphics_pipeline_layout_, context_->allocator);

  if (reflection_details_.HasUniformsOrTextures()) {
    vkDestroyDescriptorSetLayout(context_->device, descriptor_set_layout_, context_->allocator);
  }

  for (auto& shader_module : shader_modules_) {
    vkDestroyShaderModule(context_->device, shader_module.second, context_->allocator);
  }
}

void VulkanShader::Recreate(VkExtent2D render_target_extent, VkRenderPass render_pass) {
  vkDeviceWaitIdle(context_->device);

  if (reflection_details_.HasUniformsOrTextures()) {
    vkDestroyDescriptorPool(context_->device, descriptor_pool_, context_->allocator);
    uniform_buffers_.clear();
    // TODO: Destroy samplers
  }
  vkDestroyPipeline(context_->device, graphics_pipeline_, context_->allocator);

  if (reflection_details_.HasUniformsOrTextures()) {
    for (const auto& uniform : reflection_details_.uniforms) {
      uniform_buffers_.emplace(uniform.first, std::make_unique<VulkanUniformBuffer>(context_, uniform.second));
    }
    // TODO: Create samplers
  }
  CreatePipeline(render_target_extent, render_pass);
  if (reflection_details_.HasUniformsOrTextures()) {
    descriptor_pool_ = CreateDescriptorPool(context_, reflection_details_);
    descriptor_sets_ = AllocateDescriptorSets(context_, uniform_buffers_, texture_samplers_, descriptor_set_layout_, descriptor_pool_);
  }
}

// TODO: cache pipelines
void VulkanShader::CreatePipeline(VkExtent2D render_target_extent, VkRenderPass render_pass) {
  // Create the graphics pipeline, setting up all stages explicitly
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
  for (const auto& shader_module : shader_modules_) {
    VkPipelineShaderStageCreateInfo stage_info { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    stage_info.stage = shader_module.first;
    stage_info.module = shader_module.second;
    stage_info.pName = "main";
    shader_stages.push_back(stage_info);
  }

  // This is where we specify the format of the vertex data
  VkPipelineVertexInputStateCreateInfo vertex_input_info { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
  VkVertexInputBindingDescription binding_description;
  std::vector<VkVertexInputAttributeDescription> attrib_descriptions;
  if (reflection_details_.HasVertexInputs()) {
    vertex_input_info.vertexBindingDescriptionCount = 1;
    binding_description = CreateInputBindingDescription(reflection_details_.vertex_input_layout);
    vertex_input_info.pVertexBindingDescriptions = &binding_description;
    attrib_descriptions = CreateInputAttribDescriptions(reflection_details_.vertex_input_layout);
    vertex_input_info.vertexAttributeDescriptionCount = attrib_descriptions.size();
    vertex_input_info.pVertexAttributeDescriptions = attrib_descriptions.data();
  }
  else {
    vertex_input_info.vertexBindingDescriptionCount = 0;
    vertex_input_info.pVertexBindingDescriptions = nullptr;
    vertex_input_info.vertexAttributeDescriptionCount = 0;
    vertex_input_info.pVertexAttributeDescriptions = nullptr;
  }

  // The next struct tells how the vertices should be assembled into primitives
  // It is similar to choosing GL_TRIANGLES, GL_POINTS, etc
  VkPipelineInputAssemblyStateCreateInfo input_assemby_info { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
  input_assemby_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assemby_info.primitiveRestartEnable = VK_FALSE;

  // Next we describe the area of the framebuffer that we want to render to
  // Viewport defines a transform from image to framebuffer - it can stretch or squash the image
  VkViewport viewport{};
  viewport.x      = 0.0f;
  viewport.y      = 0.0f;
  viewport.width  = (float)render_target_extent.width;
  viewport.height = (float)render_target_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // Scissor defines the clip rectangle - pixels outside of the scissor range will be discarded
  VkRect2D scissor { };
  scissor.offset = { 0, 0 };
  scissor.extent = render_target_extent;

  // We combine the viewport and scissor into a VkPipelineViewportStateCreateInfo
  VkPipelineViewportStateCreateInfo viewport_state { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
  viewport_state.viewportCount = 1;
  viewport_state.pViewports = &viewport;
  viewport_state.scissorCount = 1;
  viewport_state.pScissors = &scissor;

  // The rasterizer is in charge of taking vertex shader output and turning it into fragments for the fragment shader
  // It also perfroms depth testing, face culling, and implements the scissor
  VkPipelineRasterizationStateCreateInfo rasterizer { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
  // Depth clamping sets fragments outside of the near and far clipping planes to depth equal to near and far instead
  // of discarding them. Useful for shadow mapping, and requires enabling an extension.
  rasterizer.depthClampEnable = VK_FALSE;
  // rasterizerDiscardEnable discards all fragments, disabling output to the framebuffer. I have no idea what this
  // would be used for.
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  // Possible values: VK_POLYGON_MODE_FILL, VK_POLYGON_MODE_LINE, VK_POLYGON_MODE_POINT
  // Any mode other than VK_POLYGON_MODE_FILL requires an extension
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  // Again line widths higher than 1 require an extension
  rasterizer.lineWidth = 1.0f;
  // Face culling
  if (enable_backface_cull_) {
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = front_face_ == WindingOrder::kClockwise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
  }
  else {
    rasterizer.cullMode = VK_CULL_MODE_NONE;
  }
  // No idea what depth bias does
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f; // optional
  rasterizer.depthBiasClamp = 0.0f;          // optional
  rasterizer.depthBiasSlopeFactor = 0.0f;    // optional

  // Multisampling is a possible efficient way to do antialiasing without rendering to a texture and downsampling
  // TODO: Configure this as a user-facing option
  VkPipelineMultisampleStateCreateInfo multisampling { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;          // optional
  multisampling.pSampleMask = nullptr;            // optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // optional
  multisampling.alphaToOneEnable = VK_FALSE;      // optional

  // Next we need to specify how the colours should be blended into the framebuffer after they are outputted by the
  // fragment shader
  // We do not do blending at this point, but might at some point
  VkPipelineColorBlendAttachmentState colour_blend_attachment { };
  colour_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colour_blend_attachment.blendEnable = VK_FALSE;
  colour_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
  colour_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
  colour_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;             // optional
  colour_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
  colour_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
  colour_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;             // optional

  // Here we set constants that can be used in blending calculations
  VkPipelineColorBlendStateCreateInfo colour_blending { };
  colour_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colour_blending.logicOpEnable = VK_FALSE;
  colour_blending.logicOp = VK_LOGIC_OP_COPY; // optional
  colour_blending.attachmentCount = 1;
  colour_blending.pAttachments = &colour_blend_attachment;
  colour_blending.blendConstants[0] = 0.0f; // optional
  colour_blending.blendConstants[1] = 0.0f; // optional
  colour_blending.blendConstants[2] = 0.0f; // optional
  colour_blending.blendConstants[3] = 0.0f; // optional

  // Now we have enough information to create the pipeline
  VkGraphicsPipelineCreateInfo create_info { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
  create_info.stageCount = shader_stages.size();
  create_info.pStages = shader_stages.data();
  create_info.pVertexInputState = &vertex_input_info;
  create_info.pInputAssemblyState = &input_assemby_info;
  create_info.pViewportState = &viewport_state;
  create_info.pRasterizationState = &rasterizer;
  create_info.pMultisampleState = &multisampling;
  create_info.pColorBlendState = &colour_blending;
  create_info.pDynamicState = nullptr;      // optional
  create_info.layout = graphics_pipeline_layout_;
  // We have to reference the subpass where this pipeline will be used - that would be subpass 0 as specified in the
  // CreateRenderPass function
  create_info.renderPass = render_pass;
  create_info.subpass = 0;
  // These are used when we create a new pipeline from an existing pipeline
  // This is less expensive than creating a new pipeline from scratch when you have pipelines that share features
  create_info.basePipelineHandle = VK_NULL_HANDLE; // optional
  create_info.basePipelineIndex = -1;              // optional

  // This struct must be constructed here so it doesn't go out of scope
  // TODO: Only construct this if needed
  VkPipelineDepthStencilStateCreateInfo depth_stencil { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
  depth_stencil.depthTestEnable = VK_TRUE;
  depth_stencil.depthWriteEnable = VK_TRUE;
  depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
  depth_stencil.depthBoundsTestEnable = VK_FALSE;
  depth_stencil.minDepthBounds = 0.0f; // optional
  depth_stencil.maxDepthBounds = 1.0f; // optional
  depth_stencil.stencilTestEnable = VK_FALSE;
  depth_stencil.front = {}; // optional
  depth_stencil.back = {};  // optional

  // Enable depth testing (optional)
  if (enable_depth_test_) {
    create_info.pDepthStencilState = &depth_stencil;
  }
  else {
    create_info.pDepthStencilState = nullptr;
  }

  // This can be used to create multiple pipelines
  // You can also cache pipelines and store them in a file to speed up pipeline creation
  VK_CHECK(vkCreateGraphicsPipelines(context_->device, VK_NULL_HANDLE, 1, &create_info, context_->allocator, &graphics_pipeline_));
}

void VulkanShader::Bind(VkCommandBuffer command_buffer) {
  vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline_);
  auto rt = context_->bound_render_target.lock();
  vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline_layout_, 0, 1, &descriptor_sets_[rt->GetCurrentFrameIndex()], 0, nullptr);
}

void VulkanShader::UploadUniform(int binding, void* data) {
  uniform_buffers_.at(binding)->UploadData(data);
}

void VulkanShader::BindTexture(int binding, const std::shared_ptr<Texture>& texture) {
 // TODO
}

#pragma warning(pop)

}
