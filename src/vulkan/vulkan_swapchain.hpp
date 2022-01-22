#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace cl::Vulkan {

struct VulkanWindowData;

struct VulkanSwapchain {
  void CreateSwapchain(const VulkanWindowData& window_data);
  void DestroySwapchain(const VulkanWindowData& window_data);
  void RecreateSwapchain(const VulkanWindowData& window_data);

  VkSwapchainKHR swapchain;
  VkExtent2D swapchain_extent;
  VkFormat swapchain_image_format;
  uint32_t swapchain_image_count;
  std::vector<VkImage> swapchain_images;
  std::vector<VkImageView> swapchain_image_views;

  bool enable_depth_test;
};

}
