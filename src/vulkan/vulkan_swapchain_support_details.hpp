#pragma once

#include <vector>

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace cl::Vulkan {

// This is supposed to be transient - you'll construct one of these in place whenever it is needed, use it, then discard it
struct VulkanSwapchainSupportDetails {
  VulkanSwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

  VkSurfaceFormatKHR ChooseBestSurfaceFormat() const;
  VkPresentModeKHR ChooseBestPresentMode(bool use_vsync) const;
  VkExtent2D ChooseSwapExtent(GLFWwindow* glfw_window) const;

  VkSurfaceCapabilitiesKHR surface_capabilities;
  std::vector<VkSurfaceFormatKHR> surface_formats;
  std::vector<VkPresentModeKHR> present_modes;
};

}