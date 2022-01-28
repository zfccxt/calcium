#pragma once

#include <vector>

#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

VkPhysicalDevice ChoosePhysicalDevice(const VulkanContextData& window_data, VkSurfaceKHR temp_surface);

extern const std::vector<const char*> kDeviceExtensions;

}
