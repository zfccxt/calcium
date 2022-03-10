#include "vulkan_device.hpp"

#include <cstdint>
#include <set>
#include <vector>

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_instance.hpp"
#include "vulkan/vulkan_physical_device.hpp"
#include "vulkan/vulkan_queue_family_indices.hpp"

namespace cl::vulkan {

VkDevice CreateDevice(const VulkanContextData& context_data, VkSurfaceKHR temp_surface) {
  CALCIUM_PROFILE_FUNCTION();

  VulkanQueueFamilyIndices indices(context_data.physical_device, temp_surface);

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

  // Here we enable extended dynamic state
  // This is a core feature as of Vulkan 1.3 (25th Jan 2022) but not widely supported yet
  VkPhysicalDeviceExtendedDynamicStateFeaturesEXT dynamic_state { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT };
  dynamic_state.extendedDynamicState = true;

  VkPhysicalDeviceFeatures2 device_features { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
  device_features.features.samplerAnisotropy = VK_TRUE;
  device_features.pNext = &dynamic_state;

  // Now we can create the device
  VkDeviceCreateInfo create_info { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
  create_info.queueCreateInfoCount = (uint32_t)queue_create_infos.size();
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.enabledExtensionCount = (uint32_t)kDeviceExtensions.size();
  create_info.ppEnabledExtensionNames = kDeviceExtensions.data();
  create_info.pNext = &device_features;

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
  VK_CHECK(vkCreateDevice(context_data.physical_device, &create_info, context_data.allocator, &device));
  return device;
}

void DestroyDevice(const VulkanContextData& context_data, VkDevice device) {
  vkDestroyDevice(device, context_data.allocator);
}

VkQueue FindGraphicsQueue(const VulkanContextData& context_data, VkSurfaceKHR temp_surface) {
  CALCIUM_PROFILE_FUNCTION();

  VulkanQueueFamilyIndices indices(context_data.physical_device, temp_surface);
  VkQueue graphics_queue;
  vkGetDeviceQueue(context_data.device, indices.graphics_family, 0, &graphics_queue);
  return graphics_queue;
}

VkQueue FindPresentQueue(const VulkanContextData& context_data, VkSurfaceKHR temp_surface) {
  CALCIUM_PROFILE_FUNCTION();

  VulkanQueueFamilyIndices indices(context_data.physical_device, temp_surface);
  VkQueue present_queue;
  vkGetDeviceQueue(context_data.device, indices.present_family, 0, &present_queue);
  return present_queue;
}

}
