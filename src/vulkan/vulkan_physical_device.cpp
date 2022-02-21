#include "vulkan_physical_device.hpp"

#include <assert.h>
#include <cstdio>
#include <map>
#include <set>
#include <string>

#include "vulkan/vulkan_swapchain_support_details.hpp"
#include "vulkan/vulkan_queue_family_indices.hpp"

namespace cl::Vulkan {

// We required VK_KHR_portability_subset for MoltenVK
#ifdef CALCIUM_PLATFORM_MACOS
  const std::vector<const char*> kDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset" };
#else
  const std::vector<const char*> kDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
#endif

bool CheckDeviceExtensionSupport(VkPhysicalDevice physical_device) {
  // We get a list of all extensions supported by the device and populate a vector with it
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

  // Next we create a set of required device extensions
  std::set<std::string> required_extensions(kDeviceExtensions.begin(), kDeviceExtensions.end());
  
  // We iterate over the list of required extensions, making sure each one is in the list of supported extensions
  for (const auto& extension : available_extensions) {
    required_extensions.erase(extension.extensionName);
  }

  // If the set of required extensions is empty, that means all required extensions are supported
  return required_extensions.empty();
}


int RateDeviceSuitability(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  // TODO: Improve this function

  // Some systems might have more than one Vulkan capable GPU. Here we give devices a score based on their suitability.
  // This way we can pick the best available GPU.
  VkPhysicalDeviceProperties device_properties;
  vkGetPhysicalDeviceProperties(physical_device, &device_properties);
  
  // First we score the device zero if it fails to meet the basic needs of our application
  // A device may be Vulkan capable, but only support compute. Check that the device supports graphics.
  VulkanQueueFamilyIndices indices(physical_device, surface);
  if (indices.graphics_family == -1) {
    return 0;
  }
  // Also check that the device supports presentation to a surface
  if (indices.present_family == -1) {
    return 0;
  }
  // The device must have support for the VK_KHR_swapchain extension, as well as any others we use in the future
  if (!CheckDeviceExtensionSupport(physical_device)) {
    return 0;
  }
  // The device's swapchain must also be compatible with the window surface we intend to render to
  // This must be done after making sure the device supports VK_KHR_swapchain.
  // We accept any device that supports at least one present mode, and at least one surface format.
  VulkanSwapchainSupportDetails swapchain_support(physical_device, surface);
  if (swapchain_support.surface_formats.empty() || swapchain_support.present_modes.empty()) {
    return 0;
  }

  int score = 0;

  // Ideally we would like to use a discrete GPU
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }
  
  // Devices that support larger textures ~might~ be better on average? 
  score += device_properties.limits.maxImageDimension2D / 100;

  // Devices with more memory tend to be better
  VkPhysicalDeviceMemoryProperties memory_props { };
  vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_props);
  for (size_t i = 0; i < memory_props.memoryHeapCount; ++i) {
    score += (int)memory_props.memoryHeaps[i].size / 10000000;
  }

  // Devices that handle graphics and presentation on the same queue tend to have better perfomance
  if (indices.present_family == indices.graphics_family) {
    score += 1000;
  }
  
  // Anisotropy is an optional feature - if the device doesn't support it we don't use it, but
  // we would prefer to have that feature
  VkPhysicalDeviceFeatures device_features{};
  vkGetPhysicalDeviceFeatures(physical_device, &device_features);
  if (!device_features.samplerAnisotropy) {
    score += 400;
  }

  return score;
}

VkPhysicalDevice ChoosePhysicalDevice(const VulkanContextData& context_data, VkSurfaceKHR temp_surface) {
  // First call to vkEnumeratePhysicalDevices tells us how many devices are present
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(context_data.instance, &device_count, nullptr);

  // If no Vulkan capable devices are detected, we should roll back to another graphics API
  assert(device_count > 0);

  // Second call to vkEnumeratePhysicalDevices lists the devices
  std::vector<VkPhysicalDevice> physical_devices(device_count);
  vkEnumeratePhysicalDevices(context_data.instance, &device_count, physical_devices.data());

  // Create a map to rank GPUs and populate it with scores
  std::multimap<int, VkPhysicalDevice> candidates;
  for (const auto& physical_device : physical_devices) {
    int score = RateDeviceSuitability(physical_device, temp_surface);
    candidates.insert(std::make_pair(score, physical_device));
  }

  // If the highest-rated GPU has a score of zero, we have failed to find a suitable GPU and must  roll back to another
  // graphics API. The map cannot be empty since we have already checked that at least one device is present.
  VkPhysicalDevice physical_device;
  if (candidates.rbegin()->first > 0) {
    physical_device = candidates.rbegin()->second;
  }
  else {
    // Failed to find a suitable GPU
    assert(false);
  }

#ifdef CALCIUM_BUILD_VERBOSE
  // In verbose mode, print out the name of the chosen device
  VkPhysicalDeviceProperties device_properties;
  vkGetPhysicalDeviceProperties(physical_device, &device_properties);
  int score = RateDeviceSuitability(physical_device, temp_surface);

  printf("Chose physical device: %s (Device score: %d)\n", device_properties.deviceName, score);
#endif

  return physical_device;
}

}
