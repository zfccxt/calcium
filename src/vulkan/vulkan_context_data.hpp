#pragma once

#include <vulkan/vulkan.h>

struct VulkanContextData {
  VkInstance instance;
  VkAllocationCallbacks* allocator;
#ifdef CALCIUM_BUILD_DEBUG
  VkDebugUtilsMessengerEXT debug_messenger;
#endif
};
