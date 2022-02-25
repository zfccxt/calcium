#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

uint32_t FindMemoryType(VulkanContextData* context, uint32_t type_filter, VkMemoryPropertyFlags props);
VkBuffer CreateBuffer(VulkanContextData* context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& buffer_memory);
void CopyBuffer(VulkanContextData* context, VkBuffer src_buffer, VkBuffer dest_buffer, VkDeviceSize size);

}
