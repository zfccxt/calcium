#include "vulkan_render_command_buffers.hpp"

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_constants.hpp"
#include "vulkan/vulkan_swapchain_render_pass.hpp"
#include "vulkan/vulkan_window_data.hpp"

namespace cl::Vulkan {

void VulkanRenderCommandBuffers::CreateRenderCommandBuffers(const VulkanWindowData& window_data) {
  CALCIUM_PROFILE_FUNCTION();

  render_command_buffers.resize(kMaxFramesInFlight, VK_NULL_HANDLE);
}

void VulkanRenderCommandBuffers::DestroyRenderCommandBuffers(const VulkanWindowData& window_data) {
  vkDeviceWaitIdle(window_data.context_data->device);

  vkFreeCommandBuffers(window_data.context_data->device, window_data.context_data->command_pool, kMaxFramesInFlight, render_command_buffers.data());
}

VkCommandBuffer VulkanRenderCommandBuffers::BeginRenderCommandBuffer(VulkanWindowData& window_data) {
  CALCIUM_PROFILE_FUNCTION();

  // Move to next command buffer, along with its associated sync objects
  current_command_buffer_index = (current_command_buffer_index + 1) % kMaxFramesInFlight;

  vkAcquireNextImageKHR(window_data.context_data->device, window_data.swapchain.swapchain, UINT64_MAX, 
    window_data.swapchain.sync_objects.image_available_semaphores[current_command_buffer_index], VK_NULL_HANDLE, &swapchain_image_index);

  // Check if a previous frame is using this image (i.e. there is its fence to wait on)
  if (window_data.swapchain.sync_objects.swapchain_image_fences[swapchain_image_index] != VK_NULL_HANDLE) {
    vkWaitForFences(window_data.context_data->device, 1, &window_data.swapchain.sync_objects.swapchain_image_fences[swapchain_image_index], VK_TRUE, UINT64_MAX);
  }
  // Mark the image as now being in use by this frame
  window_data.swapchain.sync_objects.swapchain_image_fences[swapchain_image_index] = window_data.swapchain.sync_objects.frame_complete_fences[current_command_buffer_index];

  // Deallocate the old command buffer
  vkWaitForFences(window_data.context_data->device, 1, &window_data.swapchain.sync_objects.frame_complete_fences[current_command_buffer_index], VK_TRUE, UINT64_MAX);
  vkFreeCommandBuffers(window_data.context_data->device, window_data.context_data->command_pool, 1, &render_command_buffers[current_command_buffer_index]);

  VkCommandBufferAllocateInfo alloc_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = window_data.context_data->command_pool;
  alloc_info.commandBufferCount = 1;

  // Allocate the new command buffer
  VK_CHECK(vkAllocateCommandBuffers(window_data.context_data->device, &alloc_info, &render_command_buffers[current_command_buffer_index]));

  // We record the command buffer straight away
  VkCommandBufferBeginInfo begin_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  VK_CHECK(vkBeginCommandBuffer(render_command_buffers[current_command_buffer_index], &begin_info));

  RecordBeginRenderPassCommand(window_data.swapchain, render_command_buffers[current_command_buffer_index], swapchain_image_index);

  current_command_buffer = render_command_buffers[current_command_buffer_index];
  return current_command_buffer;
}

void VulkanRenderCommandBuffers::EndAndSubmitRenderCommandBuffer(VulkanWindowData& window_data) {
  CALCIUM_PROFILE_FUNCTION();

  RecordEndRenderPassCommand(render_command_buffers[current_command_buffer_index]);

  VK_CHECK(vkEndCommandBuffer(render_command_buffers[current_command_buffer_index]));

  VkSubmitInfo submit_info { VK_STRUCTURE_TYPE_SUBMIT_INFO };

  VkSemaphore wait_semaphores[] = { window_data.swapchain.sync_objects.image_available_semaphores[current_command_buffer_index] };
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;

  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &render_command_buffers[current_command_buffer_index];

  VkSemaphore signal_semaphores[] = { window_data.swapchain.sync_objects.render_complete_semaphores[current_command_buffer_index] };
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  vkResetFences(window_data.context_data->device, 1, &window_data.swapchain.sync_objects.frame_complete_fences[current_command_buffer_index]);
  VK_CHECK(vkQueueSubmit(window_data.context_data->graphics_queue, 1, &submit_info, window_data.swapchain.sync_objects.frame_complete_fences[current_command_buffer_index]));

  // Present the frame
  VkPresentInfoKHR present_info { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;

  VkSwapchainKHR swapchains[] = { window_data.swapchain.swapchain };
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swapchains;
  present_info.pImageIndices = &swapchain_image_index;
  present_info.pResults = nullptr; // Optional

  vkQueuePresentKHR(window_data.context_data->present_queue, &present_info);
}

}
