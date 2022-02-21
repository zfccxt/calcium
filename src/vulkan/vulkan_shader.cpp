#include "vulkan_shader.hpp"

#include "vulkan_shader_module.hpp"

namespace cl::Vulkan {

VulkanShader::VulkanShader(VulkanContextData* context, const ShaderCreateInfo& shader_info) : context_(context) {
  ShaderCodeMap code_map = ReadAllSpvFiles(shader_info);
  // Create shader modules
  for (auto& code : code_map) {
    shader_modules_.emplace(FindVulkanShaderStage(code.first), CreateShaderModule(context, code.second));
  }

  reflection_details_.Reflect(code_map);
  // TODO: Create descriptor set layout
  // TODO: Create uniforms & samplers

  // Create pipeline layout
  CreatePipeline();

  // TODO: Create descriptor pool
  // TODO: Allocate descriptor sets
}

void VulkanShader::Recreate() {
  // TODO: DestroyDescriptorPool();
  DestroyPipeline();
  // TODO: DestroyUniforms();

  // TODO: CreateUniforms();
  CreatePipeline();
  // TODO: CreateDescriptorPool();
  // TODO: AllocateDescriptorSets();
}

void VulkanShader::CreatePipeline() {
  // TODO
}

void VulkanShader::DestroyPipeline() {
  vkDeviceWaitIdle(context_->device);

  // TODO: vkDestroyPipeline(context_->device, graphics_pipeline_, context_->allocator);
}

VulkanShader::~VulkanShader() {
  vkDeviceWaitIdle(context_->device);

  // Destroy all shader modules
  for (auto& shader_module : shader_modules_) {
    vkDestroyShaderModule(context_->device, shader_module.second, context_->allocator);
  }


  // TODO: Destroy descriptor pool
  // TODO: Destroy uniform buffers

  DestroyPipeline();

  // TODO: Destroy pipeline layout
  // TODO: Destroy descriptor set layout
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

}
