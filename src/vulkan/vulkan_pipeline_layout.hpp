#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

VkPipelineLayout CreatePipelineLayout(VulkanContextData* context, VkDescriptorSetLayout descriptor_set_layout);

}
