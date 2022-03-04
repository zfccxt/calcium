#pragma once

#include "vulkan/vulkan_context_data.hpp"

namespace cl::vulkan {

VkDebugUtilsMessengerEXT CreateDebugMessenger(const VulkanContextData& context_data);
void DestroyDebugMessenger(const VulkanContextData& context_data, VkDebugUtilsMessengerEXT debug_messenger);

}
