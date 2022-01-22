#pragma once

#include "glfw_window.hpp"
#include "vulkan/vulkan_window_data.hpp"
#include "vulkan/vulkan_swapchain.hpp"

namespace cl::Vulkan {

class VulkanWindow : public GlfwWindow {
public:
  VulkanWindow(VulkanContextData* context_data, const WindowCreateInfo& create_info);
  virtual ~VulkanWindow();

private:
  VulkanWindowData window_data_;
};

}
