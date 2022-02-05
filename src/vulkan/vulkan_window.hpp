#pragma once

#include "glfw_window.hpp"
#include "vulkan/vulkan_window_data.hpp"
#include "vulkan/vulkan_swapchain.hpp"

namespace cl::Vulkan {

class VulkanWindow : public GlfwWindow {
public:
  VulkanWindow(VulkanContextData* context_data, WindowCreateInfo create_info);
  virtual ~VulkanWindow();

  virtual void Clear() override;
  virtual void SwapBuffers() override;

  virtual void SetClearColour(const Colour& colour) override;

private:
  VulkanWindowData window_data_;
};

}
