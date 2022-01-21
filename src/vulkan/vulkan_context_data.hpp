#pragma once

#include <vulkan/vulkan.h>

struct VulkanInstanceData {
  VkInstance instance;
  VkAllocationCallbacks* allocator;
};