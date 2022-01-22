#pragma once

#include "vulkan/vulkan_swapchain.hpp"

namespace cl::Vulkan {

VkRenderPass CreateSwapchainRenderPass(const VulkanSwapchain& swapchain);
void DestroySwapchainRenderPass(const VulkanSwapchain& swapchain, VkRenderPass render_pass);

}
