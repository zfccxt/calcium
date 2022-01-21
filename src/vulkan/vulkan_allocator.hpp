#pragma once

#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

VkAllocationCallbacks* CreateAllocator(const VulkanContextData& context_data);
void DestroyAllocator(const VulkanContextData& context_data, VkAllocationCallbacks* allocator);

}
