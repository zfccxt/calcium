#include "vulkan_swapchain_support_details.hpp"

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <limits>

#include <GLFW/glfw3.h>

#include "vulkan/vulkan_check.hpp"

namespace cl::Vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

VulkanSwapchainSupportDetails::VulkanSwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  // Query the physical device's surface capabilities
  VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities));

  // Next retrieve a list of supported surface formats
  uint32_t format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
  if (format_count != 0) {
    surface_formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, surface_formats.data());
  }

  // Finally do the same for present modes
  uint32_t present_mode_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
  if (present_mode_count != 0) {
    present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data());
  }
}

VkSurfaceFormatKHR VulkanSwapchainSupportDetails::ChooseBestSurfaceFormat() const {
  // TODO: Rank available surface formats and pick the best

  // If SRBG is available, that is what we will use
  for (const auto& format : surface_formats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  // Otherwise just pick the first available surface format
  return surface_formats[0];
}

VkPresentModeKHR VulkanSwapchainSupportDetails::ChooseBestPresentMode(bool enable_vsync) const {
  // VK_PRESENT_MODE_FIFO_KHR waits for a vertical blank. This should be the correct way of doing vsync across all
  // platforms that support Vulkan.
  // VK_PRESENT_MODE_FIFO_KHR is guaranteed to be available as long as the physical device is capable of presentation
  if (enable_vsync) {
    return VK_PRESENT_MODE_FIFO_KHR;
  }

  // VK_PRESENT_MODE_MAILBOX_KHR implements triple buffering, and should result in smoother graphics
  for (const auto& present_mode : present_modes) {
    if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return present_mode;
    }
  }
  
  // If VK_PRESENT_MODE_MAILBOX_KHR is not available, we can use VK_PRESENT_MODE_IMMEDIATE_KHR to disable vsync,
  // though this is likely to result in visible tearing.
  // TODO: VK_PRESENT_MODE_MAILBOX_KHR is sometimes reported as unavailable on AMD graphics cards - this may be due to a
  // driver bug.
  for (const auto& present_mode : present_modes) {
    if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
      return present_mode;
    }
  }

  assert(false);
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchainSupportDetails::ChooseSwapExtent(GLFWwindow* glfw_window) const {
  // The swap extent is the resolution of the swapchain images
  // Vulkan will try to choose this for us based on the details it already got about the surface
  // If it was successful, we just use what it already chose
  if (surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return surface_capabilities.currentExtent;
  }
  else {
    // Otherwise, query the size of the window framebuffer and use that as the swap extent
    int width, height;
    glfwGetFramebufferSize(glfw_window, &width, &height);
    
    VkExtent2D actual_extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

    // Make sure the size returned is within the range of possible values the surface supports
    actual_extent.width  = std::clamp(actual_extent.width,  surface_capabilities.minImageExtent.width,  surface_capabilities.maxImageExtent.width);
    actual_extent.height = std::clamp(actual_extent.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);

    return actual_extent;
  }
}

#pragma warning(pop)

}
