#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

namespace cl::vulkan {

class VulkanShader;
class VulkanTexture;
class VulkanWindow;

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
  VulkanTexture* blank_texture_array;

  // Shaders must be destroyed and recreated on window or framebuffer resize
  std::vector<std::weak_ptr<VulkanShader>> loaded_shaders_;
};

}
