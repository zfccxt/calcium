#include "vulkan_render_pass.hpp"

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_image_utils.hpp"
#include "vulkan/vulkan_window_data.hpp"

namespace cl::vulkan {

VkRenderPass CreateSwapchainRenderPass(const VulkanSwapchain& swapchain) {
  return CreateRenderPass(swapchain.window_data->context_data->device, swapchain.window_data->context_data->allocator,
    swapchain.image_format, FindDepthFormat(swapchain.window_data->context_data));
}

VkRenderPass CreateRenderPass(VkDevice device, VkAllocationCallbacks* allocator, VkFormat format, VkFormat depth_format) {
  CALCIUM_PROFILE_FUNCTION();

  // TODO: Depth buffers are always enabled, but in the future I might want to optimise this
  constexpr bool enable_depth_test = true;

  // Render pass objects describe framebuffer attachments - how many colour and depth buffers there are, how to use 
  // them, how many samples, etc.

  std::vector<VkAttachmentDescription> attachment_descriptions;

  // We have either only a single colour attachment, or a colour attachment and a depth attachment
  VkAttachmentDescription colour_attachment { };
  colour_attachment.format = format;
  // If we were to set up MSAA we would need to change this
  colour_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  // We can have the framebuffer preserve its contents between render passes if we like, however we will not do this
  colour_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colour_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  // We are not yet using the stencil buffer, so these next items are irrelevant
  colour_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colour_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colour_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colour_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  attachment_descriptions.push_back(colour_attachment);

  VkAttachmentReference colour_attachment_ref {};
  // attachment = index of associated VkAttachmentDescription in attachment descriptions array
  colour_attachment_ref.attachment = 0;
  colour_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass { };
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colour_attachment_ref;

  // Only used if depth testing is enabled
  // TODO: Only construct this if needed
  VkAttachmentReference depth_attachment_ref { };
  depth_attachment_ref.attachment = 1;
  depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  // No point using a depth buffer if depth testing is not enabled
  if (enable_depth_test) {
    VkAttachmentDescription depth_attachment { };
    depth_attachment.format = depth_format;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachment_descriptions.push_back(depth_attachment);
  
    subpass.pDepthStencilAttachment = &depth_attachment_ref;
  }

  // Subpass dependencies are a way of specifying which subpasses rely on which operations to have been completed
  // before they can begin. We create a dependency here to say that the subpass that draws colours to the framebuffer
  // can only begin when the VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT stage has begun
  VkSubpassDependency dependency { };
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcAccessMask = 0;

  if (enable_depth_test) {
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  }
  else {
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  }

  VkRenderPassCreateInfo render_pass_info { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
  render_pass_info.attachmentCount = (uint32_t)attachment_descriptions.size();
  render_pass_info.pAttachments = attachment_descriptions.data();
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies = &dependency;
  
  VkRenderPass render_pass;
  VK_CHECK(vkCreateRenderPass(device, &render_pass_info, allocator, &render_pass));
  return render_pass;
}

void RecordBeginSwapchainRenderPassCommand(VulkanSwapchain& swapchain, VkCommandBuffer command_buffer, int swapchain_image_index) {
  CALCIUM_PROFILE_FUNCTION();

 VkRenderPassBeginInfo render_pass_info { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
  render_pass_info.renderPass = swapchain.render_pass;
  render_pass_info.framebuffer = swapchain.framebuffers[swapchain_image_index];
  render_pass_info.renderArea.offset = { 0, 0 };
  render_pass_info.renderArea.extent = swapchain.extent;
  render_pass_info.clearValueCount = (uint32_t)swapchain.clear_values.size();
  render_pass_info.pClearValues = swapchain.clear_values.data();

  vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
}

// std::shared_ptr<Shader> VulkanContext::CreateShader(const ShaderCreateInfo& shader_info) {
//   auto window = context_data_.bound_render_target.lock();
//   VkExtent2D framebuffer_extent = window->GetFramebufferExtent();
//   VkRenderPass render_pass = window->GetRenderPass();
//   bool enable_depth_test = window->IsDepthTestEnabled();
//   bool enable_backface_cull = window->IsBackfaceCullingEnabled();
//   WindingOrder front_face = window->GetPolygonFrontFace();
//   auto shader = std::make_shared<VulkanShader>(&context_data_, shader_info, enable_depth_test, enable_backface_cull, front_face, framebuffer_extent, render_pass);
//   context_data_.loaded_shaders_.push_back(shader);
//   return shader;
// }


}
