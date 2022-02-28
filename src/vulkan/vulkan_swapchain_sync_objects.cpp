#include "vulkan_swapchain_sync_objects.hpp"

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_constants.hpp"
#include "vulkan/vulkan_window_data.hpp"

namespace cl::Vulkan {

void VulkanSwapchainSyncObjects::CreateSwapchainSyncObjects(const VulkanWindowData& window_data) {
  CALCIUM_PROFILE_FUNCTION();

  image_available_semaphores.resize(kMaxFramesInFlight);
  render_complete_semaphores.resize(kMaxFramesInFlight);
  frame_complete_fences.resize(kMaxFramesInFlight);

  VkSemaphoreCreateInfo semaphore_info { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
  VkFenceCreateInfo fence_info { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    VK_CHECK(vkCreateSemaphore(window_data.context_data->device, &semaphore_info, window_data.context_data->allocator, &image_available_semaphores[i]));
    VK_CHECK(vkCreateSemaphore(window_data.context_data->device, &semaphore_info, window_data.context_data->allocator, &render_complete_semaphores[i]));
    VK_CHECK(vkCreateFence(window_data.context_data->device, &fence_info, window_data.context_data->allocator, &frame_complete_fences[i]));
  }

  swapchain_image_fences.resize(window_data.swapchain.image_count, VK_NULL_HANDLE);
}

void VulkanSwapchainSyncObjects::DestroySwapchainSyncObjects(const VulkanWindowData& window_data) {
  CALCIUM_PROFILE_FUNCTION();

  vkDeviceWaitIdle(window_data.context_data->device);

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    vkDestroySemaphore(window_data.context_data->device, image_available_semaphores[i], window_data.context_data->allocator);
    vkDestroySemaphore(window_data.context_data->device, render_complete_semaphores[i], window_data.context_data->allocator);
    vkDestroyFence(window_data.context_data->device, frame_complete_fences[i], window_data.context_data->allocator);
  }
}

}
