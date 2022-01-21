#pragma once

#include "vulkan/vulkan_context_data.hpp"

struct GLFWwindow;

namespace cl::Vulkan {

struct VulkanWindowData {
  VulkanContextData* context_data;
  GLFWwindow* glfw_window;
  VkSurfaceKHR surface;
  VkPhysicalDevice physical_device;
  VkDevice device;
};

}
