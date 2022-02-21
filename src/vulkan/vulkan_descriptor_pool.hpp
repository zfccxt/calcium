#pragma once

#include <vulkan/vulkan.h>

#include "shader_reflection_details.hpp"
#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

VkDescriptorPool CreateDescriptorPool(VulkanContextData* context, const ShaderReflectionDetails& reflection_details);

}
