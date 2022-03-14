#include "vulkan_texture.hpp"

#include <assert.h>

#include <stb_image.h>
#include <vulkan/vulkan.h>

#include "instrumentor.hpp"
#include "texture_utils.hpp"
#include "vulkan_buffer_utils.hpp"
#include "vulkan_check.hpp"
#include "vulkan_image_utils.hpp"

namespace cl::vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

VkFilter FindVkFilter(TextureFilter filter) {
  switch (filter) {
    case TextureFilter::kLinear:  return VK_FILTER_LINEAR;
    case TextureFilter::kNearest: return VK_FILTER_NEAREST;
    default:                      return VK_FILTER_MAX_ENUM;
  }
}

VkSamplerAddressMode FindSamplerAddressMode(TextureWrap wrap) {
  switch (wrap) {
    case TextureWrap::kRepeat:         return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case TextureWrap::kMirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case TextureWrap::kClampToEdge:    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case TextureWrap::kClampToBorder:  return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    default:                           return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
  }
}

VulkanTexture::VulkanTexture(VulkanContextData* context, const TextureArrayCreateInfo& texture_array_info) : context_(context) {
  auto raw_arr = LoadRawTextureArray(texture_array_info);

  width_ = raw_arr->width;
  height_ = raw_arr->height;
  depth_ = raw_arr->depth;

  CreateTexture(raw_arr->data, raw_arr->width, raw_arr->height, raw_arr->depth, true, texture_array_info.filter, texture_array_info.wrap);
}

VulkanTexture::VulkanTexture(VulkanContextData* context, const TextureCreateInfo& texture_info) : context_(context) {
  CALCIUM_PROFILE_FUNCTION();

  // Load texture into CPU memory
  int width, height, channels;
  stbi_set_flip_vertically_on_load(texture_info.flip_vertical_on_load);
  stbi_uc* pixels = stbi_load(texture_info.file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  assert(pixels);
  
  CreateTexture(pixels, width, height, 1, false, texture_info.filter, texture_info.wrap);

  // Delete texture from CPU
  stbi_image_free(pixels);
}

VulkanTexture::VulkanTexture(VulkanContextData* context, const BlankTextureCreateInfo& texture_info, bool is_array) : context_(context) {
  CALCIUM_PROFILE_FUNCTION();

  size_t data_size = (size_t)texture_info.width * texture_info.height;
	uint32_t* pixels = new uint32_t[data_size];
	// TODO: Why is this backwards?
	uint32_t colour = texture_info.colour.UintABGR();
	for (size_t i = 0; i < data_size; ++i) {
		pixels[i] = colour;
	}

  CreateTexture(pixels, texture_info.width, texture_info.height, 1, is_array, texture_info.filter, texture_info.wrap);

  delete[] pixels;
}

void VulkanTexture::CreateTexture(void* pixels, int width, int height, int layers, bool is_array, TextureFilter filter, TextureWrap wrap) {
  CALCIUM_PROFILE_FUNCTION();

  width_ = width;
  height_ = height;
  depth_ = layers;

  VkDeviceSize image_size = (VkDeviceSize)width * (VkDeviceSize)height * (VkDeviceSize)layers * sizeof(uint32_t);

  // Create staging buffer
  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_memory;
  CreateBuffer(context_, image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    staging_buffer, staging_buffer_memory);
  
  // Copy texture to GPU staging buffer
  void* data;
  vkMapMemory(context_->device, staging_buffer_memory, 0, image_size, 0, &data);
  memcpy(data, pixels, (size_t)image_size);
  vkUnmapMemory(context_->device, staging_buffer_memory);
  
  uint32_t mip_levels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
  
  // Create image to store texture data
  const VkImageType image_type = is_array ?  VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_2D; // TODO: Check
  CreateImage(context_, width, height, layers, image_type, VK_FORMAT_R8G8B8A8_UNORM,
    VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture_image_, texture_image_memory_, mip_levels);
  
  // Copy texture data from staging buffer to image
  TransitionImageLayout(context_, texture_image_, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mip_levels, layers);
  CopyBufferToImage(context_, staging_buffer, texture_image_, (uint32_t)width, (uint32_t)height, (uint32_t)layers);
  // GenerateMipmaps implicitly performs a layout transition to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
  // TODO: Check whether we need to account for depth here
  GenerateMipmaps(context_, texture_image_, VK_FORMAT_R8G8B8A8_UNORM, width, height, mip_levels, layers);

  // Destroy staging buffer
  vkDestroyBuffer(context_->device, staging_buffer, context_->allocator);
  vkFreeMemory(context_->device, staging_buffer_memory, context_->allocator);
  
  const VkImageViewType view_type = is_array ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
  texture_image_view_ = CreateImageView(context_, texture_image_, view_type, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, mip_levels, layers);
  
  // Create sampler
  VkSamplerCreateInfo sampler_info { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
  
  // Set filters from texture_info
  VkFilter vkfilter = FindVkFilter(filter);
  sampler_info.minFilter = vkfilter;
  sampler_info.magFilter = vkfilter;
  
  // Set address mode from texture_info
  VkSamplerAddressMode address_mode = FindSamplerAddressMode(wrap);
  sampler_info.addressModeU = address_mode;
  sampler_info.addressModeV = address_mode;
  sampler_info.addressModeW = address_mode;
  
  // Check whether the device supports anisotropy
  VkPhysicalDeviceFeatures device_features { };
  vkGetPhysicalDeviceFeatures(context_->physical_device, &device_features);
  if (device_features.samplerAnisotropy) {
    // Query physical device to find max anisotropy and use that
    VkPhysicalDeviceProperties properties { };
    vkGetPhysicalDeviceProperties(context_->physical_device, &properties);
    sampler_info.anisotropyEnable = VK_TRUE;
    sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  }
  else {
    // If the device doesn't support anisotropy, just disable it
    sampler_info.anisotropyEnable = VK_FALSE;
    sampler_info.maxAnisotropy = 1.0f;
  }
  
  sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  sampler_info.unnormalizedCoordinates = VK_FALSE;
  
  // compareEnable could be useful for percentage closer filtering
  sampler_info.compareEnable = VK_FALSE;
  sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
  
  sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  sampler_info.mipLodBias = 0.0f;
  sampler_info.minLod = 0.0f;
  sampler_info.maxLod = static_cast<float>(mip_levels);
  
  VK_CHECK(vkCreateSampler(context_->device, &sampler_info, context_->allocator, &texture_sampler_));
}

VulkanTexture::~VulkanTexture() {
  CALCIUM_PROFILE_FUNCTION();

  vkDeviceWaitIdle(context_->device);

  vkDestroySampler(context_->device, texture_sampler_, context_->allocator);
  vkDestroyImageView(context_->device, texture_image_view_, context_->allocator);
  vkDestroyImage(context_->device, texture_image_, context_->allocator);
  vkFreeMemory(context_->device, texture_image_memory_, context_->allocator);
}

size_t VulkanTexture::GetWidth() const {
  return width_;
}

size_t VulkanTexture::GetHeight() const {
  return height_;
}

size_t VulkanTexture::GetDepth() const {
  return depth_;
}

#pragma warning(pop)

}
