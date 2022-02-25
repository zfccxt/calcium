#pragma once

#include <vulkan/vulkan.h>

#include "texture.hpp"
#include "texture_create_info.hpp"
#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanTexture : public Texture {
public:
  VulkanTexture(VulkanContextData* context, const TextureCreateInfo& texture_info);
  ~VulkanTexture();

  virtual void Bind() override;

private:
  void CreateTexture(void* pixels, int width, int height, TextureFilter filter, TextureWrap wrap);

private:
  VulkanContextData* context_;

  VkImage texture_image_;
  VkDeviceMemory texture_image_memory_;
  VkImageView texture_image_view_;
  VkSampler texture_sampler_;
};

}