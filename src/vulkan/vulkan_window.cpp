#include "vulkan_window.hpp"

#include <GLFW/glfw3.h>

#include "vulkan/vulkan_command_pool.hpp"
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_physical_device.hpp"
#include "vulkan/vulkan_window_surface.hpp"

namespace cl::Vulkan {

VulkanWindow::VulkanWindow(VulkanContextData* context_data, const WindowCreateInfo& create_info) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  CreateGlfwWindow(create_info);

  window_data_.context_data = context_data;
  window_data_.glfw_window = glfw_window_;

  window_data_.surface = CreateWindowSurface(window_data_);
  window_data_.physical_device = ChoosePhysicalDevice(window_data_);
  window_data_.device = CreateDevice(window_data_);
  window_data_.graphics_queue = FindGraphicsQueue(window_data_);
  window_data_.present_queue = FindPresentQueue(window_data_);
  window_data_.command_pool = CreateCommandPool(window_data_);
}

VulkanWindow::~VulkanWindow() {
  DestroyCommandPool(window_data_, window_data_.command_pool);
  DestroyDevice(window_data_, window_data_.device);
  DestroyWindowSurface(window_data_, window_data_.surface);
}

}
