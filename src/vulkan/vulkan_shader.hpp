#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>

#include "buffer_layout.hpp"
#include "shader.hpp"
#include "shader_create_info.hpp"
#include "vulkan_context_data.hpp"
#include "vulkan_texture.hpp"
#include "vulkan_uniform_buffer.hpp"
#include "winding_order.hpp"

namespace cl::Vulkan {

class VulkanShader : public Shader {
public:
  VulkanShader(VulkanContextData* context, const ShaderCreateInfo& shader_info, bool enable_depth_test, bool enable_backface_cull,
    WindingOrder front_face, VkExtent2D render_target_extent, VkRenderPass render_pass);
  virtual ~VulkanShader();

  virtual void UploadUniform(int binding, void* data) override;
  virtual void BindTexture(int binding, const std::shared_ptr<Texture>& texture) override;

public:
  void Recreate(VkExtent2D render_target_extent, VkRenderPass render_pass);

  void Bind(VkCommandBuffer command_buffer);

private:
  void CreatePipeline(VkExtent2D render_target_extent, VkRenderPass render_pass);

  void CreateUniforms();
  void DestroyUniforms();

private:
  VulkanContextData* context_;
  bool enable_depth_test_;
  bool enable_backface_cull_;
  WindingOrder front_face_;

  VkPipeline graphics_pipeline_;
  VkPipelineLayout graphics_pipeline_layout_;

  std::unordered_map<VkShaderStageFlagBits, VkShaderModule> shader_modules_;

  VkDescriptorSetLayout descriptor_set_layout_ = VK_NULL_HANDLE;
  VkDescriptorPool descriptor_pool_;
  std::vector<VkDescriptorSet> descriptor_sets_;
  VulkanUniformMap uniform_buffers_;
  VulkanTextureMap bound_textures_;
};

}
