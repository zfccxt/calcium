#pragma once

#include "glfw_window.hpp"

namespace cl::Vulkan {

class VulkanWindow : public GlfwWindow {
public:
  VulkanWindow(const WindowCreateInfo& create_info);
};

}
