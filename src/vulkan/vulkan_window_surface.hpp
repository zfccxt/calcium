#pragma once

#include "vulkan/vulkan_window_data.hpp"

namespace cl::vulkan {

VkSurfaceKHR CreateWindowSurface(const VulkanWindowData& window_data);
void DestroyWindowSurface(const VulkanWindowData& window_data, VkSurfaceKHR surface);

}
