#pragma once

#include <cstdint>
#include <vector>

#include "vulkan/vulkan_swapchain_sync_objects.hpp"

namespace cl::Vulkan {

struct VulkanWindowData;

struct VulkanSwapchain {
  void CreateSwapchain();
  void DestroySwapchain();
  void RecreateSwapchain();

  void CreateSwapchainFramebuffers();
  void DestroySwapchainFramebuffers();

  // TODO: Colour class
  void SetClearValue(float r, float g, float b, float a);

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

  bool enable_depth_test;
};

}
