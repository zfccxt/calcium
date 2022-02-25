#pragma on

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

void CreateImage(VulkanContextData* context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& image_memory);
bool HasStencilComponent(VkFormat format);
void TransitionImageLayout(VulkanContextData* context, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout);
void CopyBufferToImage(VulkanContextData* context, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
VkFormat FindSupportedFormat(VulkanContextData* context, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
VkFormat FindDepthFormat(VulkanContextData* context);
VkImageView CreateImageView(VulkanContextData* context, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags);

}
