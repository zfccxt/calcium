#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace cl::Vulkan {

struct VulkanWindowData;

struct VulkanRenderCommandBuffers {
  void CreateRenderCommandBuffers(const VulkanWindowData& window_data);
  void DestroyRenderCommandBuffers(const VulkanWindowData& window_data);

  VkCommandBuffer BeginRenderCommandBuffer(VulkanWindowData& window_data);
  void EndAndSubmitRenderCommandBuffer(VulkanWindowData& window_data);

  std::vector<VkCommandBuffer> render_command_buffers_;
  uint32_t swapchain_image_index_;
  uint32_t current_command_buffer_index_ = 0;
};

}
