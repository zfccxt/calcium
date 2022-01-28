#pragma once

#include <vulkan/vulkan.h>

namespace cl::Vulkan {

struct VulkanContextData {
  VkInstance instance;
  VkAllocationCallbacks* allocator;
#ifdef CALCIUM_BUILD_DEBUG
  VkDebugUtilsMessengerEXT debug_messenger;
#endif

  VkPhysicalDevice physical_device;
  VkDevice device;
  VkQueue graphics_queue;
  VkQueue present_queue;
  VkCommandPool command_pool;
};

}
