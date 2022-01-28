#pragma once

#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

VkCommandPool CreateCommandPool(const VulkanContextData& context_data, VkSurfaceKHR temp_surface);
void DestroyCommandPool(const VulkanContextData& context_data, VkCommandPool command_pool);

}
