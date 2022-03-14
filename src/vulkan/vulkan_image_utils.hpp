#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan_context_data.hpp"

namespace cl::vulkan {

void CreateImage(VulkanContextData* context, uint32_t width, uint32_t height, uint32_t layers, VkImageType type, VkFormat format, VkImageTiling tiling,
    VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& image_memory, uint32_t mip_levels);
bool HasStencilComponent(VkFormat format);
void TransitionImageLayout(VulkanContextData* context, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, uint32_t mip_levels, uint32_t layers);
void CopyBufferToImage(VulkanContextData* context, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t depth);
VkFormat FindSupportedFormat(VulkanContextData* context, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
VkFormat FindDepthFormat(VulkanContextData* context);
VkImageView CreateImageView(VulkanContextData* context, VkImage image, VkImageViewType type, VkFormat format, VkImageAspectFlags aspect_flags, uint32_t mip_levels, uint32_t layers);
void GenerateMipmaps(VulkanContextData* context, VkImage image, VkFormat image_format, uint32_t width, uint32_t height, uint32_t mip_levels, uint32_t layers);

}
