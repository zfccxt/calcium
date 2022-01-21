#pragma once

#include <vulkan/vulkan.h>

struct VulkanContextData {
  VkInstance instance;
  VkAllocationCallbacks* allocator;
};
