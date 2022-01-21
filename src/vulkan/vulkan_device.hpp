#pragma once

#include "vulkan/vulkan_window_data.hpp"

namespace cl::Vulkan {

VkDevice CreateDevice(const VulkanWindowData& window_data);
void DestroyDevice(const VulkanWindowData& window_data, VkDevice device);

}
