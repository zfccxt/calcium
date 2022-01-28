#pragma once

#include "vulkan/vulkan_context_data.hpp"
#include "vulkan/vulkan_swapchain.hpp"
#include "vulkan/vulkan_render_command_buffers.hpp"

struct GLFWwindow;

namespace cl::Vulkan {

struct VulkanWindowData {
  VulkanContextData* context_data;
  GLFWwindow* glfw_window;
  bool enable_vsync;

  VkSurfaceKHR surface;
  VulkanSwapchain swapchain;
  VulkanRenderCommandBuffers render_command_buffers;
};

}
