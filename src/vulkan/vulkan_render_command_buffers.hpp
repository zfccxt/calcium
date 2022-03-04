#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace cl::vulkan {

struct VulkanWindowData;

// TODO: Support multiple render passes
struct VulkanRenderCommandBuffers {
  void CreateRenderCommandBuffers(const VulkanWindowData& window_data);
  void DestroyRenderCommandBuffers(const VulkanWindowData& window_data);

  VkCommandBuffer BeginRenderCommandBuffer(VulkanWindowData& window_data);
  void EndAndSubmitRenderCommandBuffer(VulkanWindowData& window_data);

  std::vector<VkCommandBuffer> render_command_buffers;
  VkCommandBuffer current_command_buffer = VK_NULL_HANDLE;
  uint32_t swapchain_image_index;
  uint32_t current_command_buffer_index = 0;
};

}
