#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace cl::vulkan {

// This is supposed to be transient - you'll construct one of these in place whenever it is needed, use it, then discard it
struct VulkanQueueFamilyIndices {
  VulkanQueueFamilyIndices(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

  uint32_t graphics_family = -1;
  uint32_t present_family = -1;
};

}
