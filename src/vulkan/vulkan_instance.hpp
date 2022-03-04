#pragma once

#include <vector>

#include "vulkan/vulkan_context_data.hpp"

namespace cl::vulkan {

VkInstance CreateInstance(VulkanContextData& context_data);
void DestroyInstance(VulkanContextData& context_data, VkInstance instance);

extern const std::vector<const char*> kValidationLayers;

}
