#pragma once

#include <vulkan/vulkan.h>

#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

// We pass the VkDescriptorSetLayout by pointer since it can be null, i.e. when the shader contains no uniform buffers or texture samplers
VkPipelineLayout CreatePipelineLayout(VulkanContextData* context, VkDescriptorSetLayout descriptor_set_layout);

}
