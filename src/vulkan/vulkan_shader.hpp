#pragma once

#include <unordered_map>

#include <vulkan/vulkan.h>

#include "shader.hpp"
#include "shader_create_info.hpp"
#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanShader : public Shader {
public:
  VulkanShader(VulkanContextData* context, const ShaderCreateInfo& shader_info);
  virtual ~VulkanShader();

  virtual void Bind() override;

  virtual void UploadUniform(int binding, void* data) override;
  virtual void BindTexture(int binding, const std::shared_ptr<Texture>& texture) override;

public:
  void Recreate();

private:
  void CreatePipeline();
  void DestroyPipeline();

private:
  VulkanContextData* context_;

  VkPipeline graphics_pipeline_;
  VkPipelineLayout graphics_pipeline_layout_;

  std::unordered_map<VkShaderStageFlagBits, VkShaderModule> shader_modules_;
  VkDescriptorSetLayout descriptor_set_layout_;
};

}
