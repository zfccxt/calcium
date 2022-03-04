#pragma once

#include <vulkan/vulkan.h>

#include "shader_reflection_details.hpp"
#include "shader_stage.hpp"
#include "vulkan_context_data.hpp"

namespace cl::vulkan {

VkShaderModule CreateShaderModule(VulkanContextData* context, const SpvCode& code);
VkShaderStageFlagBits FindVulkanShaderStage(ShaderStage stage);

}
