#pragma once

#include "vulkan/vulkan_window_data.hpp"

namespace cl::Vulkan {

VkCommandPool CreateCommandPool(const VulkanWindowData& window_data);
void DestroyCommandPool(const VulkanWindowData& window_data, VkCommandPool command_pool);

}
