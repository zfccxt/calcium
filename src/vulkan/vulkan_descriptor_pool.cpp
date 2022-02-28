#include "vulkan_descriptor_pool.hpp"

#include <array>

#include "instrumentor.hpp"
#include "vulkan_check.hpp"
#include "vulkan_constants.hpp"
#include "vulkan_texture.hpp"

namespace cl::Vulkan {

VkDescriptorPool CreateDescriptorPool(VulkanContextData* context, const ShaderReflectionDetails& reflection_details) {
  CALCIUM_PROFILE_FUNCTION();

  std::vector<VkDescriptorPoolSize> pool_sizes(reflection_details.uniforms.size() + reflection_details.textures.size());

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

std::vector<VkDescriptorSet> AllocateDescriptorSets(VulkanContextData* context, const VulkanUniformMap& uniform_buffers,
    const VulkanTextureMap& texture_samplers, VkDescriptorSetLayout descriptor_set_layout, VkDescriptorPool descriptor_pool) {
  CALCIUM_PROFILE_FUNCTION();

  std::vector<VkDescriptorSet> descriptor_sets(kMaxFramesInFlight);

  std::vector<VkDescriptorSetLayout> layouts(kMaxFramesInFlight, descriptor_set_layout);

  VkDescriptorSetAllocateInfo alloc_info { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
  alloc_info.descriptorPool = descriptor_pool;
  alloc_info.descriptorSetCount = kMaxFramesInFlight;
  alloc_info.pSetLayouts = layouts.data();
  VK_CHECK(vkAllocateDescriptorSets(context->device, &alloc_info, descriptor_sets.data()));

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {

    for (const auto& uniform : uniform_buffers) {
      VkDescriptorBufferInfo buffer_info { };
      buffer_info.buffer = uniform.second->buffers[i];
      buffer_info.offset = 0; // TODO: check this
      buffer_info.range = uniform.second->size; // VK_WHOLE_SIZE might also be possible?

      VkWriteDescriptorSet write { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
      write.dstSet = descriptor_sets[i];
      write.dstBinding = uniform.first;
      write.dstArrayElement = 0;
      write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      write.descriptorCount = 1;
      write.pBufferInfo = &buffer_info;

      vkUpdateDescriptorSets(context->device, 1, &write, 0, nullptr);
    }

    for (const auto& sampler : texture_samplers) {
      VkDescriptorImageInfo image_info { };
      image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      image_info.imageView = sampler.second->GetImageView();
      image_info.sampler = sampler.second->GetSampler();

      VkWriteDescriptorSet write { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
      write.dstSet = descriptor_sets[i];
      write.dstBinding = sampler.first;
      write.dstArrayElement = 0;
      write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      write.descriptorCount = 1;
      write.pImageInfo = &image_info;

      vkUpdateDescriptorSets(context->device, 1, &write, 0, nullptr);
    }

  }

  return descriptor_sets;
}

}
