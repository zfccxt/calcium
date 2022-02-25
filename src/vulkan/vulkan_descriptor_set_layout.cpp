#include "vulkan_descriptor_set_layout.hpp"

#include <vector>

#include "vulkan_check.hpp"
#include "vulkan_shader_module.hpp"

namespace cl::Vulkan {

VkDescriptorSetLayout CreateDescriptorSetLayout(VulkanContextData* context, const ShaderReflectionDetails& reflection_details) {
  std::vector<VkDescriptorSetLayoutBinding> layouts(reflection_details.uniforms.size() + reflection_details.textures.size());

  size_t i = 0;
  for (const auto& uniform : reflection_details.uniforms) {
    // This refers to the layout(binding = n) in the vertex shader
    layouts[i].binding = (uint32_t)uniform.first;
    layouts[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layouts[i].descriptorCount = 1;
    layouts[i].stageFlags = FindVulkanShaderStage(uniform.second.stage);
    layouts[i].pImmutableSamplers = nullptr;

    ++i;
  }

  for (const auto& uniform : reflection_details.textures) {
    layouts[i].binding = (uint32_t)uniform.first;
    layouts[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layouts[i].descriptorCount = 1;
    layouts[i].stageFlags = FindVulkanShaderStage(uniform.second.stage);
    layouts[i].pImmutableSamplers = nullptr;

    ++i;
  }

  VkDescriptorSetLayoutCreateInfo layout_info { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
  layout_info.bindingCount = (uint32_t)layouts.size();
  layout_info.pBindings = layouts.data();

  VkDescriptorSetLayout descriptor_set_layout;
  VK_CHECK(vkCreateDescriptorSetLayout(context->device, &layout_info, context->allocator, &descriptor_set_layout));
  return descriptor_set_layout;
}

}
