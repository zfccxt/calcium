#pragma once

#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>

#include "buffer_layout.hpp"
#include "shader.hpp"
#include "shader_create_info.hpp"
#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanShader : public Shader {
public:
  VulkanShader(VulkanContextData* context, const ShaderCreateInfo& shader_info, VkExtent2D render_target_extent, VkRenderPass render_pass, bool enable_depth_test);
  virtual ~VulkanShader();

  virtual void Bind() override;

  virtual void UploadUniform(int binding, void* data) override;
  virtual void BindTexture(int binding, const std::shared_ptr<Texture>& texture) override;

public:
  void Recreate(VkExtent2D render_target_extent, VkRenderPass render_pass, bool enable_depth_test);

private:
  void CreatePipeline(VkExtent2D render_target_extent, VkRenderPass render_pass, bool enable_depth_test);

private:
  VulkanContextData* context_;

  VkPipeline graphics_pipeline_;
  VkPipelineLayout graphics_pipeline_layout_;

  std::unordered_map<VkShaderStageFlagBits, VkShaderModule> shader_modules_;
  VkDescriptorSetLayout descriptor_set_layout_;
  VkDescriptorPool descriptor_pool_;
  std::vector<VkDescriptorSet> descriptor_sets_;
};

}
