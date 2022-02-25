#include "vulkan_image_utils.hpp"

#include <assert.h>

#include "vulkan_buffer_utils.hpp"
#include "vulkan_check.hpp"
#include "vulkan_command_buffer_utils.hpp"

namespace cl::Vulkan {

void CreateImage(VulkanContextData* context, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
                VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& image_memory) {
  // Create texture image
  VkImageCreateInfo image_info { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.extent.width = (uint32_t)width;
  image_info.extent.height = (uint32_t)height;
  image_info.extent.depth = 1;
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  // TODO: Implement list of fallbacks. VK_FORMAT_R8G8B8A8_SRGB is widely supported but not guaranteed on all platforms.
  image_info.format = format;
  image_info.tiling = tiling;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  image_info.usage = usage;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.flags = 0; // optional
  VK_CHECK(vkCreateImage(context->device, &image_info, context->allocator, &image));
  
  // Allocate memory for the texture image
  VkMemoryRequirements memory_requirements;
  vkGetImageMemoryRequirements(context->device, image, &memory_requirements);
  VkMemoryAllocateInfo allocInfo { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
  allocInfo.allocationSize = memory_requirements.size;
  allocInfo.memoryTypeIndex = FindMemoryType(context, memory_requirements.memoryTypeBits, properties);
  vkAllocateMemory(context->device, &allocInfo, context->allocator, &image_memory);

  vkBindImageMemory(context->device, image, image_memory, 0);
}

bool HasStencilComponent(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void TransitionImageLayout(VulkanContextData* context, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout) {
  VkCommandBuffer command_buffer = BeginSingleUseCommandBuffer(context);

  VkImageMemoryBarrier barrier { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
  barrier.oldLayout = old_layout;
  barrier.newLayout = new_layout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (HasStencilComponent(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  }
  else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  VkPipelineStageFlags source_stage;
  VkPipelineStageFlags destination_stage;
  if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  }
  else {
    assert(false);
  }

  vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

  EndAndSubmitSingleUseCommandBuffer(context, command_buffer);
}

void CopyBufferToImage(VulkanContextData* context, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
  VkCommandBuffer command_buffer = BeginSingleUseCommandBuffer(context);

  VkBufferImageCopy region { };
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;

  region.imageOffset = {0, 0, 0};
  region.imageExtent = { width, height, 1 };
  
  vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  EndAndSubmitSingleUseCommandBuffer(context, command_buffer);
}

VkFormat FindSupportedFormat(VulkanContextData* context, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(context->physical_device, format, &props);
    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      return format;
    }
    else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  assert(false);
  return VK_FORMAT_MAX_ENUM;
}

VkFormat FindDepthFormat(VulkanContextData* context) {
  return FindSupportedFormat(context, { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                               VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkImageView CreateImageView(VulkanContextData* context, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags) {
  VkImageViewCreateInfo view_info { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
  view_info.image = image;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = format;
  view_info.subresourceRange.aspectMask = aspect_flags;
  view_info.subresourceRange.baseMipLevel = 0;
  view_info.subresourceRange.levelCount = 1;
  view_info.subresourceRange.baseArrayLayer = 0;
  view_info.subresourceRange.layerCount = 1;
  
  VkImageView image_view;
  VK_CHECK(vkCreateImageView(context->device, &view_info, context->allocator, &image_view));
  return image_view;
}

}
