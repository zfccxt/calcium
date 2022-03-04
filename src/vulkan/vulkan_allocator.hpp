#pragma once

#include "vulkan/vulkan_context_data.hpp"

namespace cl::vulkan {

VkAllocationCallbacks* CreateAllocator(const VulkanContextData& context_data);
void DestroyAllocator(const VulkanContextData& context_data, VkAllocationCallbacks* allocator);

}
