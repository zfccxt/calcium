#pragma once

#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.h>

#include "texture.hpp"
#include "texture_create_info.hpp"
#include "vulkan_context_data.hpp"

namespace cl::vulkan {

class VulkanTexture : public Texture {
public:
  VulkanTexture(VulkanContextData* context, const TextureCreateInfo& texture_info);
  VulkanTexture(VulkanContextData* context, const BlankTextureCreateInfo& texture_info);
  ~VulkanTexture();

private:
  void CreateTexture(void* pixels, int width, int height, TextureFilter filter, TextureWrap wrap);

public:
  inline VkImageView GetImageView() const { return texture_image_view_; }
  inline VkSampler   GetSampler()   const { return texture_sampler_; }

private:
  VulkanContextData* context_;

  VkImage texture_image_;
  VkDeviceMemory texture_image_memory_;
  VkImageView texture_image_view_;
  VkSampler texture_sampler_;
};

typedef std::unordered_map<size_t, VulkanTexture*> VulkanTextureMap;

}