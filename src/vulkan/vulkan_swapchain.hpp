#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace cl::Vulkan {

struct VulkanWindowData;

struct VulkanSwapchain {
  void CreateSwapchain();
  void DestroySwapchain();
  void RecreateSwapchain();

  void CreateSwapchainFramebuffers();
  void DestroySwapchainFramebuffers();

  VulkanWindowData* window_data;

  VkSwapchainKHR swapchain;
  VkExtent2D swapchain_extent;
  VkFormat swapchain_image_format;
  uint32_t swapchain_image_count;
  std::vector<VkImage> swapchain_images;
  std::vector<VkImageView> swapchain_image_views;
  VkRenderPass swapchain_render_pass;
  std::vector<VkFramebuffer> swapchain_framebuffers;

  bool enable_depth_test;
};

}
