#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "colour.hpp"
#include "vulkan/vulkan_depth_buffer.hpp"
#include "vulkan/vulkan_swapchain_sync_objects.hpp"

namespace cl::vulkan {

struct VulkanWindowData;

struct VulkanSwapchain {
  void CreateSwapchain();
  void DestroySwapchain();
  void RecreateSwapchain();

  void CreateSwapchainFramebuffers();
  void DestroySwapchainFramebuffers();

  void SetClearValue(const Colour& colour);

  VulkanWindowData* window_data;

  VkSwapchainKHR swapchain;
  VkExtent2D extent;
  VkFormat image_format;
  uint32_t image_count;
  std::vector<VkImage> images;
  std::vector<VkImageView> image_views;
  VkRenderPass render_pass;
  std::vector<VkFramebuffer> framebuffers;
  std::vector<VkClearValue> clear_values;

  VulkanSwapchainSyncObjects sync_objects;

  // Managed with new and delete because order of destruction is important
  // basically a unique_ptr
  VulkanDepthBuffer* depth_buffer;
};

}
