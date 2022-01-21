#include "vulkan_window.hpp"

#include <GLFW/glfw3.h>

#include "vulkan/vulkan_window_surface.hpp"

namespace cl::Vulkan {

VulkanWindow::VulkanWindow(VulkanContextData* context_data, const WindowCreateInfo& create_info) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  CreateGlfwWindow(create_info);

  window_data_.context_data = context_data;
  window_data_.glfw_window = glfw_window_;

  window_data_.surface = CreateWindowSurface(window_data_);
}

}
