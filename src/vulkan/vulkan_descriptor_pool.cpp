#include "vulkan_descriptor_pool.hpp"

#include "vulkan_check.hpp"
#include "vulkan_constants.hpp"

namespace cl::Vulkan {

VkDescriptorPool CreateDescriptorPool(VulkanContextData* context, const ShaderReflectionDetails& reflection_details) {
  std::vector<VkDescriptorPoolSize> pool_sizes(reflection_details.uniforms.size() + reflection_details.textures.size());

  // pool_info.pPoolSizes must be greater than 0, so if the shader has no uniform buffers and no texture samplers
  // we cannot create a descriptor pool 
  if (pool_sizes.size() == 0) {
    return VK_NULL_HANDLE;
  }

  size_t i = 0;
  for (const auto& uniform : reflection_details.uniforms) {
    pool_sizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
    pool_sizes[i].descriptorCount = kMaxFramesInFlight;
    ++i;
  }

  for (const auto& uniform : reflection_details.textures) {
    pool_sizes[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
    pool_sizes[i].descriptorCount = kMaxFramesInFlight;
    ++i;
  }

  VkDescriptorPoolCreateInfo pool_info { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
  pool_info.poolSizeCount = pool_sizes.size();
  pool_info.pPoolSizes = pool_sizes.data();
  pool_info.maxSets = kMaxFramesInFlight;

  VkDescriptorPool descriptor_pool;
  VK_CHECK(vkCreateDescriptorPool(context->device, &pool_info, context->allocator, &descriptor_pool));
  return descriptor_pool;
}

}
