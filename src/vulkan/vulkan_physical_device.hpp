#pragma once

#include <vector>

#include "vulkan/vulkan_window_data.hpp"

namespace cl::Vulkan {

VkPhysicalDevice ChoosePhysicalDevice(const VulkanWindowData& window_data);

extern const std::vector<const char*> kDeviceExtensions;

}
