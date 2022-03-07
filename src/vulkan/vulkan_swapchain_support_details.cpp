#include "vulkan_swapchain_support_details.hpp"

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <limits>

#include <GLFW/glfw3.h>

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"

namespace cl::vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

VulkanSwapchainSupportDetails::VulkanSwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  CALCIUM_PROFILE_FUNCTION();

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
  CALCIUM_PROFILE_FUNCTION();

  const std::vector<VkFormat> request_formats = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
  const VkColorSpaceKHR request_colour_space = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

  // Per Spec Format and View Format are expected to be the same unless VK_IMAGE_CREATE_MUTABLE_BIT was set at image creation
  // Assuming that the default behavior is without setting this bit, there is no need for separate Swapchain image and image view format
  // Additionally several new color spaces were introduced with Vulkan Spec v1.0.40,
  // hence we must make sure that a format with the mostly available color space, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, is found and used.
  
  // First check if only one format, VK_FORMAT_UNDEFINED, is available, which would imply that any format is available
  if (surface_formats.size() == 1) {
    if (surface_formats[0].format == VK_FORMAT_UNDEFINED) {
      VkSurfaceFormatKHR ret;
      ret.format = request_formats[0];
      ret.colorSpace = request_colour_space;
      return ret;
    }
    else {
      // No point in searching another format
      return surface_formats[0];
    }
  }
  else {
    // Request several formats, the first found will be used
    for (int request_i = 0; request_i < request_formats.size(); ++request_i) {
      for (uint32_t avail_i = 0; avail_i < surface_formats.size(); ++avail_i) {
        if (surface_formats[avail_i].format == request_formats[request_i] && surface_formats[avail_i].colorSpace == request_colour_space) {
          return surface_formats[avail_i];
        }
      }
    }
    
    // If none of the requested image formats could be found, use the first available
    return surface_formats[0];
  }
}

VkPresentModeKHR VulkanSwapchainSupportDetails::ChooseBestPresentMode(bool enable_vsync) const {
  CALCIUM_PROFILE_FUNCTION();

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
  CALCIUM_PROFILE_FUNCTION();

  // Query the size of the window framebuffer and use that as the swap extent
  int width, height;
  glfwGetFramebufferSize(glfw_window, &width, &height);
  
  VkExtent2D extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
  
  // Make sure the size returned is within the range of possible values the surface supports
  extent.width  = std::clamp(extent.width,  surface_capabilities.minImageExtent.width,  surface_capabilities.maxImageExtent.width);
  extent.height = std::clamp(extent.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
  
  return extent;
}

#pragma warning(pop)

}
