#pragma once

#include "vulkan/vulkan_context_data.hpp"
#include "vulkan/vulkan_swapchain.hpp"

struct GLFWwindow;

namespace cl::Vulkan {

struct VulkanWindowData {
  VulkanContextData* context_data;
  GLFWwindow* glfw_window;
  bool enable_vsync;

  VkSurfaceKHR surface;
  VkPhysicalDevice physical_device;
  VkDevice device;
  VkQueue graphics_queue;
  VkQueue present_queue;
  VkCommandPool command_pool;

  VulkanSwapchain swapchain;
};

}
