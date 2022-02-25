#pragma once

#include <vulkan/vulkan.h>

#include "shader_reflection_details.hpp"
#include "vulkan_context_data.hpp"
#include "vulkan_uniform_buffer.hpp"

namespace cl::Vulkan {

VkDescriptorPool CreateDescriptorPool(VulkanContextData* context, const ShaderReflectionDetails& reflection_details);
std::vector<VkDescriptorSet> AllocateDescriptorSets(VulkanContextData* context, const VulkanUniformMap& uniform_buffers,
    VkDescriptorSetLayout descriptor_set_layout, VkDescriptorPool descriptor_pool);

}
