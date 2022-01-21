#include "vulkan_window.hpp"

#include <GLFW/glfw3.h>

namespace cl::Vulkan {

VulkanWindow::VulkanWindow(VulkanContextData* context_data, const WindowCreateInfo& create_info) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  CreateGlfwWindow(create_info);

  window_data_.context_data = context_data;
}

}
