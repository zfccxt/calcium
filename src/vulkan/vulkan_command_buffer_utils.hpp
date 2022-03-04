#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_context_data.hpp"

namespace cl::vulkan {

VkCommandBuffer BeginSingleUseCommandBuffer(VulkanContextData* context);
void EndAndSubmitSingleUseCommandBuffer(VulkanContextData* context, VkCommandBuffer command_buffer);

}
