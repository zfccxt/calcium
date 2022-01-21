#include "vulkan_device.hpp"

#include <cstdint>
#include <set>
#include <vector>

#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_instance.hpp"
#include "vulkan/vulkan_physical_device.hpp"
#include "vulkan/vulkan_queue_family_indices.hpp"

namespace cl::Vulkan {

VkDevice CreateDevice(const VulkanWindowData& window_data) {
  VulkanQueueFamilyIndices indices(window_data.physical_device, window_data.surface);

  // First we have to configure queues
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  // Use a std::set to figure out whether we need to configure separate graphics and present queues
  std::set<uint32_t> unique_queue_families = { indices.graphics_family, indices.present_family };
  float queue_priority = 1.0f;
  
  for (uint32_t queue_family : unique_queue_families) {
    // Configure each unique queue
    VkDeviceQueueCreateInfo queue_create_info { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  // Here we can specify the list of features we will be using
  VkPhysicalDeviceFeatures device_features { };
  device_features.samplerAnisotropy = VK_TRUE;

  // Now we can create the device
  VkDeviceCreateInfo create_info { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
  create_info.queueCreateInfoCount = (uint32_t)queue_create_infos.size();
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.pEnabledFeatures = &device_features;  
  create_info.enabledExtensionCount = (uint32_t)kDeviceExtensions.size();
  create_info.ppEnabledExtensionNames = kDeviceExtensions.data();

  // Vulkan no longer makes a distinction between instance and device validation layers, so the enabledLayerCount and
  // ppEnabledLayerNames fields are ignored by up to date Vulkan implementations. We might as well set them anyway to
  // be backwards compatible.
#ifdef CALCIUM_BUILD_DEBUG
  create_info.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
  create_info.ppEnabledLayerNames = kValidationLayers.data();
#else
  create_info.enabledLayerCount = 0;
#endif

  VkDevice device;
  VK_CHECK(vkCreateDevice(window_data.physical_device, &create_info, window_data.context_data->allocator, &device));
  return device;
}

void DestroyDevice(const VulkanWindowData& window_data, VkDevice device) {
  vkDestroyDevice(device, window_data.context_data->allocator);
}

}
