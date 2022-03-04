#pragma once

#include <vector>

#include "vulkan/vulkan_context_data.hpp"

namespace cl::vulkan {

VkPhysicalDevice ChoosePhysicalDevice(const VulkanContextData& window_data, VkSurfaceKHR temp_surface);

extern const std::vector<const char*> kDeviceExtensions;

}
