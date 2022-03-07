#pragma once

#include "vulkan/vulkan_swapchain.hpp"

namespace cl::vulkan {

VkRenderPass CreateSwapchainRenderPass(const VulkanSwapchain& swapchain);
VkRenderPass CreateRenderPass(VkDevice device, VkAllocationCallbacks* allocator, VkFormat format, VkFormat depth_format, bool enable_depth_test);

void RecordBeginSwapchainRenderPassCommand(VulkanSwapchain& swapchain, VkCommandBuffer command_buffer, int swapchain_image_index);

}
