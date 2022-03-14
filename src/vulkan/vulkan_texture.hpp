#pragma once

#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.h>

#include "texture.hpp"
#include "texture_array.hpp"
#include "texture_create_info.hpp"
#include "texture_array_create_info.hpp"
#include "vulkan_context_data.hpp"

namespace cl::vulkan {

class VulkanTexture : public Texture, public TextureArray {
public:
  VulkanTexture(VulkanContextData* context, const TextureArrayCreateInfo& texture_array_info);
  VulkanTexture(VulkanContextData* context, const TextureCreateInfo& texture_info);
  VulkanTexture(VulkanContextData* context, const BlankTextureCreateInfo& texture_info, bool is_array);
  ~VulkanTexture();

  virtual size_t GetWidth() const override;
  virtual size_t GetHeight() const override;

public:
  size_t GetDepth() const;

private:
  void CreateTexture(void* pixels, int width, int height, int layers, bool is_array, TextureFilter filter, TextureWrap wrap);

public:
  inline VkImageView GetImageView() const { return texture_image_view_; }
  inline VkSampler   GetSampler()   const { return texture_sampler_; }

private:
  VulkanContextData* context_;

  VkImage texture_image_;
  VkDeviceMemory texture_image_memory_;
  VkImageView texture_image_view_;
  VkSampler texture_sampler_;

  size_t width_, height_, depth_;
};

typedef std::unordered_map<size_t, VulkanTexture*> VulkanTextureMap;

}