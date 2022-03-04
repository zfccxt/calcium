#pragma once

#include "vulkan/vulkan_swapchain.hpp"

namespace cl::vulkan {

VkRenderPass CreateSwapchainRenderPass(const VulkanSwapchain& swapchain);
void DestroySwapchainRenderPass(const VulkanSwapchain& swapchain, VkRenderPass render_pass);

void RecordBeginRenderPassCommand(VulkanSwapchain& swapchain, VkCommandBuffer command_buffer, int swapchain_image_index);
void RecordEndRenderPassCommand(VkCommandBuffer command_buffer);

}
