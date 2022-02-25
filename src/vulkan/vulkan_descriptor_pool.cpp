#include "vulkan_descriptor_pool.hpp"

#include <array>

#include "vulkan_check.hpp"
#include "vulkan_constants.hpp"

namespace cl::Vulkan {

VkDescriptorPool CreateDescriptorPool(VulkanContextData* context, const ShaderReflectionDetails& reflection_details) {
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
    VkDescriptorSetLayout descriptor_set_layout, VkDescriptorPool descriptor_pool) {
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

    //  std::vector<VkDescriptorImageInfo> image_infos;
    //  for (const auto& uniform : uniforms_) {
    //    if (uniform.second.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
    //      VkDescriptorImageInfo image_info { };
    //      image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //      VulkanTexture* texture = texture_library_.GetDefault();
    //      image_info.imageView = texture->GetTextureImageView();
    //      image_info.sampler = texture->GetTextureSampler();
    //      image_infos.push_back(image_info);
    //    }
    //  }
// 
    // descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    // descriptor_writes[1].dstSet = descriptor_sets[i];
    // descriptor_writes[1].dstBinding = 1;
    // descriptor_writes[1].dstArrayElement = 0;
    // descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // descriptor_writes[1].descriptorCount = (uint32_t)image_infos.size();
    // descriptor_writes[1].pImageInfo = image_infos.data();

  }

  return descriptor_sets;
}

}
