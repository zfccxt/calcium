#include "vulkan_queue_family_indices.hpp"

#include <vector>

#include "vulkan/vulkan_check.hpp"

namespace cl::Vulkan {

VulkanQueueFamilyIndices::VulkanQueueFamilyIndices(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  // First call to vkGetPhysicalDeviceQueueFamilyProperties finds the number of queue families
  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

  // Second call to vkGetPhysicalDeviceQueueFamilyProperties enumerates the properties of the queue families
  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

  // We need to find at least one queue family that supports VK_QUEUE_GRAPHICS_BIT and at least one family that supports
  // presentation to a surface. These queues are often the same, but might be distinct.

  // We prefer to use the same queue for both presentation and graphics, since this often results in better performance
  // For example, a device might have one queue that is capable of graphics and presentation, and another capable of
  // presentation but not graphics.
  for (uint32_t i = 0; i < queue_family_count; ++i) {
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      graphics_family = i;
    }

  // Note: the availability of a presentation queue implies that VK_KHR_swapchain must be supported
    VkBool32 present_support = false;
    VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support));
    if (present_support) {
      present_family = i;
    }

    // If we ever find a queue family that can do both graphics and presentation, we stop the search
    if (graphics_family == present_family) {
      break;
    }
  }
}

}
