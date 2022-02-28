#include "vulkan_window_surface.hpp"

#include <GLFW/glfw3.h>

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"

namespace cl::Vulkan {

VkSurfaceKHR CreateWindowSurface(const VulkanWindowData& window_data) {
  CALCIUM_PROFILE_FUNCTION();

  VkSurfaceKHR surface;
  VK_CHECK(glfwCreateWindowSurface(window_data.context_data->instance, window_data.glfw_window, window_data.context_data->allocator, &surface));
  return surface;
}

void DestroyWindowSurface(const VulkanWindowData& window_data, VkSurfaceKHR surface) {
  CALCIUM_PROFILE_FUNCTION();

  vkDestroySurfaceKHR(window_data.context_data->instance, surface, window_data.context_data->allocator);
}

}
