#pragma once

#include <memory>

#include <vulkan/vulkan.h>

namespace cl::Vulkan {

class VulkanWindow;
class VulkanTexture;

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

  // We only support windows as render targets right now, so this is a guaranteed to be a pointer to a VulkanWindow
  std::weak_ptr<VulkanWindow> bound_render_target;

  // Explicitly managed w/ new and delete because order of destruction matters
  // Probably should be a unique_ptr
  VulkanTexture* blank_texture;
};

}
