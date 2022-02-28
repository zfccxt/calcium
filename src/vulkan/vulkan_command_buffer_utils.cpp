#include "vulkan_command_buffer_utils.hpp"

#include "instrumentor.hpp"
#include "vulkan_check.hpp"

namespace cl::Vulkan {

VkCommandBuffer BeginSingleUseCommandBuffer(VulkanContextData* context) {
  CALCIUM_PROFILE_FUNCTION();

  VkCommandBufferAllocateInfo alloc_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = context->command_pool;
  alloc_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  VK_CHECK(vkAllocateCommandBuffers(context->device, &alloc_info, &command_buffer));

  VkCommandBufferBeginInfo begin_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  VK_CHECK(vkBeginCommandBuffer(command_buffer, &begin_info));

  return command_buffer;
}

void EndAndSubmitSingleUseCommandBuffer(VulkanContextData* context, VkCommandBuffer command_buffer) {
  CALCIUM_PROFILE_FUNCTION();

  VK_CHECK(vkEndCommandBuffer(command_buffer));

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &command_buffer;

  VK_CHECK(vkQueueSubmit(context->graphics_queue, 1, &submitInfo, VK_NULL_HANDLE));
  VK_CHECK(vkQueueWaitIdle(context->graphics_queue));

  // Now we can deallocate the command buffer
  vkFreeCommandBuffers(context->device, context->command_pool, 1, &command_buffer);
}

}
