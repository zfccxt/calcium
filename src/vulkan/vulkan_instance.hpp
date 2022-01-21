#pragma once

#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

VkInstance CreateInstance(VulkanContextData& context_data);
void DestroyInstance(VulkanContextData& context_data, VkInstance instance);

}
