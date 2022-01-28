#pragma once

#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

VkDevice CreateDevice(const VulkanContextData& window_data, VkSurfaceKHR temp_surface);
void DestroyDevice(const VulkanContextData& window_data, VkDevice device);

VkQueue FindGraphicsQueue(const VulkanContextData& window_data, VkSurfaceKHR temp_surface);
VkQueue FindPresentQueue(const VulkanContextData& window_data, VkSurfaceKHR temp_surface);

}
