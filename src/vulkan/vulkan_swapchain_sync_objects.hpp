#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace cl::Vulkan {

struct VulkanWindowData;

struct VulkanSwapchainSyncObjects {
  void CreateSwapchainSyncObjects(const VulkanWindowData& window_data);
  void DestroySwapchainSyncObjects(const VulkanWindowData& window_data);

  std::vector<VkSemaphore> image_available_semaphores;
  std::vector<VkSemaphore> render_complete_semaphores;
  std::vector<VkFence> frame_complete_fences;
  std::vector<VkFence> swapchain_image_fences;
};

}
