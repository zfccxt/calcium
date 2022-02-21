#include "vulkan_shader.hpp"

#include "vulkan_descriptor_pool.hpp"
#include "vulkan_descriptor_set_layout.hpp"
#include "vulkan_pipeline_layout.hpp"
#include "vulkan_shader_module.hpp"

namespace cl::Vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

VulkanShader::VulkanShader(VulkanContextData* context, const ShaderCreateInfo& shader_info) : context_(context) {
  ShaderCodeMap code_map = ReadAllSpvFiles(shader_info);
  // Create shader modules
  for (auto& code : code_map) {
    shader_modules_.emplace(FindVulkanShaderStage(code.first), CreateShaderModule(context, code.second));
  }

  reflection_details_.Reflect(code_map);
  descriptor_set_layout_ = CreateDescriptorSetLayout(context, reflection_details_);
  // TODO: Create uniforms & samplers

  graphics_pipeline_layout_ = CreatePipelineLayout(context, descriptor_set_layout_);
  CreatePipeline();

  descriptor_pool_ = CreateDescriptorPool(context, reflection_details_);
  // TODO: Allocate descriptor sets
}

void VulkanShader::Recreate() {
  vkDeviceWaitIdle(context_->device);

  vkDestroyDescriptorPool(context_->device, descriptor_pool_, context_->allocator);
  // TODO: DestroyUniforms();
  vkDestroyPipeline(context_->device, graphics_pipeline_, context_->allocator);

  // TODO: CreateUniforms();
  CreatePipeline();
  descriptor_pool_ = CreateDescriptorPool(context_, reflection_details_);
  // TODO: AllocateDescriptorSets();
}

void VulkanShader::CreatePipeline() {
  // TODO
}

VulkanShader::~VulkanShader() {
  vkDeviceWaitIdle(context_->device);

  vkDestroyDescriptorPool(context_->device, descriptor_pool_, context_->allocator);
  // TODO: Destroy uniform buffers

  vkDestroyPipeline(context_->device, graphics_pipeline_, context_->allocator);

  vkDestroyPipelineLayout(context_->device, graphics_pipeline_layout_, context_->allocator);
  vkDestroyDescriptorSetLayout(context_->device, descriptor_set_layout_, context_->allocator);

  // Destroy all shader modules
  for (auto& shader_module : shader_modules_) {
    vkDestroyShaderModule(context_->device, shader_module.second, context_->allocator);
  }
}

void VulkanShader::Bind() {
  // TODO
}

void VulkanShader::UploadUniform(int binding, void* data) {
  // TODO
}

void VulkanShader::BindTexture(int binding, const std::shared_ptr<Texture>& texture) {
 // TODO
}

#pragma warning(pop)

}
