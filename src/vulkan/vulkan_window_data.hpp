#pragma once

#include "vulkan/vulkan_context_data.hpp"
#include "vulkan/vulkan_swapchain.hpp"
#include "vulkan/vulkan_render_command_buffers.hpp"
#include "winding_order.hpp"

struct GLFWwindow;

namespace cl::vulkan {

struct VulkanWindowData {
  VulkanContextData* context_data;
  GLFWwindow* glfw_window;

  bool enable_vsync = false;
  bool enable_backface_cull = false;
  WindingOrder front_face;

  VkSurfaceKHR surface;
  VulkanSwapchain swapchain;
  VulkanRenderCommandBuffers render_command_buffers;
};

}
