#include "vulkan_depth_buffer.hpp"

#include "instrumentor.hpp"
#include "vulkan_image_utils.hpp"

namespace cl::vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

VulkanDepthBuffer::VulkanDepthBuffer(VulkanContextData* context, VkExtent2D swapchain_extent) : context_(context) {
  CALCIUM_PROFILE_FUNCTION();

  VkFormat depth_format = FindDepthFormat(context);
  CreateImage(context, swapchain_extent.width, swapchain_extent.height, depth_format,
    VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depth_image, depth_image_memory, 1);
  depth_image_view = CreateImageView(context, depth_image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
  TransitionImageLayout(context, depth_image, depth_format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}

VulkanDepthBuffer::~VulkanDepthBuffer() {
  vkDestroyImageView(context_->device, depth_image_view, context_->allocator);
  vkDestroyImage(context_->device, depth_image, context_->allocator);
  vkFreeMemory(context_->device, depth_image_memory, context_->allocator);
}

#pragma warning(pop)

}
