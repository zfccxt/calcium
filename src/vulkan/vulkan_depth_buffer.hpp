#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

struct VulkanDepthBuffer {
  VulkanDepthBuffer(VulkanContextData* context, VkExtent2D swapchain_extent);
  ~VulkanDepthBuffer();

  VkImage depth_image;
  VkDeviceMemory depth_image_memory;
  VkImageView depth_image_view;

private:
  VulkanContextData* context_;
};

}
