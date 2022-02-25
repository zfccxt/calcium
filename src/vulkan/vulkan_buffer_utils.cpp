#include "vulkan_buffer_utils.hpp"

#include <assert.h>

#include "vulkan_command_buffer_utils.hpp"
#include "vulkan_check.hpp"

namespace cl::Vulkan {

uint32_t FindMemoryType(VulkanContextData* context, uint32_t type_filter, VkMemoryPropertyFlags props) {
  // Make sure the physical device can provide memory that suits our needs
  // We provide a VkMemoryPropertyFlags with bits set based on the properties of the memory we require
  // We look through the list of available memory types on the current GPU until we find a suitable type
  VkPhysicalDeviceMemoryProperties mem_props;
  vkGetPhysicalDeviceMemoryProperties(context->physical_device, &mem_props);
  for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
    if ((type_filter & (1 << i)) && (mem_props.memoryTypes[i].propertyFlags & props) == props) {
      return i;
    }
  }
  assert(false);
  return -1;
}

VkBuffer CreateBuffer(VulkanContextData* context, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& buffer_memory) {
  VkBufferCreateInfo buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  // size in bytes of entire vertex data
  buffer_info.size = size;
  // This is a vertex buffer
  buffer_info.usage = usage;
  // We are only going to use this data in the graphics queue and no other queues
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  // flags can configure sparse buffer memory, whatever that is
  buffer_info.flags = 0;

  VK_CHECK(vkCreateBuffer(context->device, &buffer_info, context->allocator, &buffer));

  // The buffer has now been created, but it has no memory assigned to it yet
  // We must allocate some memory, so first we find out how much we need and what type
  VkMemoryRequirements mem_requirements;
  vkGetBufferMemoryRequirements(context->device, buffer, &mem_requirements);

  VkMemoryAllocateInfo alloc_info { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
  alloc_info.allocationSize = mem_requirements.size;
  alloc_info.memoryTypeIndex = FindMemoryType(context, mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  VK_CHECK(vkAllocateMemory(context->device, &alloc_info, context->allocator, &buffer_memory));

  // Memory offset can be set to 0 since we are not dealing with separate vertex buffers
  VK_CHECK(vkBindBufferMemory(context->device, buffer, buffer_memory, 0));

  return buffer;
}

void CopyBuffer(VulkanContextData* context, VkBuffer src_buffer, VkBuffer dest_buffer, VkDeviceSize size) {
  // We can use the graphics queue for transfer operations, since the Vulkan spec tells us that any queue capable of
  // graphics must also be capable of transfer

  // Memory transfer operations are done using command buffers
  VkCommandBuffer command_buffer = BeginSingleUseCommandBuffer(context);

  VkBufferCopy copy_region{};
  copy_region.srcOffset = 0; // optional
  copy_region.dstOffset = 0; // optional
  copy_region.size = size;
  vkCmdCopyBuffer(command_buffer, src_buffer, dest_buffer, 1, &copy_region);

  EndAndSubmitSingleUseCommandBuffer(context, command_buffer);
}

}
