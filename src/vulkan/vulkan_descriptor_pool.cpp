#include "vulkan_descriptor_pool.hpp"

#include <array>

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

std::vector<VkDescriptorSet> AllocateDescriptorSets(VulkanContextData* context, VkDescriptorSetLayout descriptor_set_layout, VkDescriptorPool descriptor_pool) {
  std::vector<VkDescriptorSet> descriptor_sets(kMaxFramesInFlight);

  // If the shader contains no uniform buffers or texture samplers then the descriptor pool cannot be allocated
  // Therefore we return an empty vector of descriptor sets
  if (!descriptor_pool) {
    return descriptor_sets;
  }

  std::vector<VkDescriptorSetLayout> layouts(kMaxFramesInFlight, descriptor_set_layout);

  VkDescriptorSetAllocateInfo alloc_info { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
  alloc_info.descriptorPool = descriptor_pool;
  alloc_info.descriptorSetCount = kMaxFramesInFlight;
  alloc_info.pSetLayouts = layouts.data();
  VK_CHECK(vkAllocateDescriptorSets(context->device, &alloc_info, descriptor_sets.data()));

  /*
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    std::vector<VkDescriptorBufferInfo> buffer_infos;
    for (const auto& uniform : reflection_details.uniforms) {
      VkDescriptorBufferInfo buffer_info { };
      buffer_info.buffer = uniform.second.buffers[i];
      buffer_info.offset = 0; // TODO: check this
      buffer_info.range = uniform.second.size;
      buffer_infos.push_back(buffer_info);
    }

    std::vector<VkDescriptorImageInfo> image_infos;
    for (const auto& uniform : uniforms_) {
      VkDescriptorImageInfo image_info { };
      image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      image_info.imageView = texture->GetTextureImageView();
      image_info.sampler = texture->GetTextureSampler();
      image_infos.push_back(image_info);
    }

    std::array<VkWriteDescriptorSet, 2> descriptor_writes { };

    descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].dstSet = descriptor_sets[i];
    descriptor_writes[0].dstBinding = 0;
    descriptor_writes[0].dstArrayElement = 0;
    descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount = (uint32_t)buffer_infos.size();
    descriptor_writes[0].pBufferInfo = buffer_infos.data();

    descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].dstSet = descriptor_sets[i];
    descriptor_writes[1].dstBinding = 1;
    descriptor_writes[1].dstArrayElement = 0;
    descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptor_writes[1].descriptorCount = (uint32_t)image_infos.size();
    descriptor_writes[1].pImageInfo = image_infos.data();

    vkUpdateDescriptorSets(context->device, (uint32_t)descriptor_writes.size(), descriptor_writes.data(), 0, nullptr);
  }
  */

  return descriptor_sets;
}

}
