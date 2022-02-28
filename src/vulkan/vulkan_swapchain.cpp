#include "vulkan_swapchain.hpp"

#include <GLFW/glfw3.h>

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_swapchain_render_pass.hpp"
#include "vulkan/vulkan_swapchain_support_details.hpp"
#include "vulkan/vulkan_queue_family_indices.hpp"
#include "vulkan/vulkan_window_data.hpp"

namespace cl::Vulkan {

#pragma warning(push)
#pragma warning(disable: 26812)

void VulkanSwapchain::CreateSwapchain() {
  CALCIUM_PROFILE_FUNCTION();

  VulkanSwapchainSupportDetails swapchain_support(window_data->context_data->physical_device, window_data->surface);
  VkSurfaceFormatKHR surface_format = swapchain_support.ChooseBestSurfaceFormat();
  VkPresentModeKHR present_mode = swapchain_support.ChooseBestPresentMode(window_data->enable_vsync);
  extent = swapchain_support.ChooseSwapExtent(window_data->glfw_window);
  image_format = surface_format.format;

  // It's  recommended to request at least one more swapchain image than the minimum supported, as this avoids having
  // to wait for the driver to complete internal operations before we can acquire the next image to render to
  image_count = swapchain_support.surface_capabilities.minImageCount + 1;

  // Make sure we do not exceed the maximum number of swapchain images
  // 0 is a special value indicating that there is no maximum
  if (swapchain_support.surface_capabilities.maxImageCount > 0 && image_count > swapchain_support.surface_capabilities.maxImageCount) {
    image_count = swapchain_support.surface_capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR create_info { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
  create_info.surface = window_data->surface;
  create_info.minImageCount = image_count;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = extent;

  // imageArrayLayers greater than 1 can be used for things like stereoscopic 3D
  create_info.imageArrayLayers = 1;

  // We are rendering directly to this swapchain - if we were rendering to a texture and performing postprocessing
  // maybe we would use VK_IMAGE_USAGE_TRANSFER_DST_BIT
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  // Next we must specify how to handle swapchain images that will be used across multiple queue families. If so, we
  // would need to draw on the images in the graphics queue, then submit them on the present queue.
  VulkanQueueFamilyIndices indices(window_data->context_data->physical_device, window_data->surface);
  uint32_t queue_family_indices[] = { indices.graphics_family, indices.present_family };

  if (indices.graphics_family == indices.present_family) {
    // VK_SHARING_MODE_EXCLUSIVE offers the best performance
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // No need to specify queueFamilyIndexCount or pQueueFamilyIndices as Vulkan assumes by default that you will be
    // using the same queue for both graphics and presentation
  }
  else {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
  }

  // To specify that we don't want a transform (such as a 90 degree rotation or horizontal flip), we can set this to
  // the current transform supplied by default
  create_info.preTransform = swapchain_support.surface_capabilities.currentTransform;

  // compositeAlpha is used to specify whether the alpha channel should be used for blending with other windows in the
  // window system. We don't need this, so we can ignore the alpha channel.
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  create_info.presentMode = present_mode;

  // clipped means we don't care about the colour of pixels that are obscured, for example by another window
  create_info.clipped = VK_TRUE;

  create_info.oldSwapchain = VK_NULL_HANDLE;

  VK_CHECK(vkCreateSwapchainKHR(window_data->context_data->device, &create_info, window_data->context_data->allocator, &swapchain));

  // We only ever specified a minimum number of swapchain images, so it's possible the swapchain we have created has
  // more. Therefore we need to query the swapchain to find out how many images it contains.
  // We don't need to destroy the images, since they are implicity destroyed when we destroy the swapchain - we are
  // only retrieving image handles
  vkGetSwapchainImagesKHR(window_data->context_data->device, swapchain, &image_count, nullptr);
  images.resize(image_count);
  vkGetSwapchainImagesKHR(window_data->context_data->device, swapchain, &image_count, images.data());

  // Create image views
  image_views.resize(images.size());
  for (size_t i = 0; i < images.size(); ++i) {
    VkImageViewCreateInfo create_info { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    create_info.image = images[i];

    // We want to treat each image as a 2D texture
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = image_format;

    // Use default mapping for colour channels
    // Note: Swizzling other than VK_COMPONENT_SWIZZLE_IDENTITY is not available on some incomplete Vulkan implementations, for example MoltenVK
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    // subresourceRange describes image use, as well as which part of the image to access
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(window_data->context_data->device, &create_info, window_data->context_data->allocator, &image_views[i]));
  }

  if (window_data->enable_depth_test) {
    depth_buffer = new VulkanDepthBuffer(window_data->context_data, extent);
  }
}

void VulkanSwapchain::DestroySwapchain() {
  CALCIUM_PROFILE_FUNCTION();

  vkDeviceWaitIdle(window_data->context_data->device);

  if (window_data->enable_depth_test) {
    delete depth_buffer;
  }

  for (auto image_view : image_views) {
    vkDestroyImageView(window_data->context_data->device, image_view, window_data->context_data->allocator);
  }

  vkDestroySwapchainKHR(window_data->context_data->device, swapchain, window_data->context_data->allocator);
}

void VulkanSwapchain::RecreateSwapchain() {
  CALCIUM_PROFILE_FUNCTION();

 // If the window is minimized, wait until it is unminimized
  int width = 0, height = 0;
  glfwGetFramebufferSize(window_data->glfw_window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(window_data->glfw_window, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(window_data->context_data->device);

  DestroySwapchainFramebuffers();
  DestroySwapchainRenderPass(*this, render_pass);
  DestroySwapchain();

  CreateSwapchain();
  render_pass = CreateSwapchainRenderPass(*this);
  CreateSwapchainFramebuffers();
}

void VulkanSwapchain::CreateSwapchainFramebuffers() {
  CALCIUM_PROFILE_FUNCTION();

  framebuffers.resize(image_views.size());
  
  // Each image view needs its own framebuffer
  for (size_t i = 0; i < image_views.size(); ++i) {
    std::vector<VkImageView> attachments = { image_views[i] };

    if (enable_depth_test) {
      attachments.push_back(depth_buffer->depth_image_view);
    }

    VkFramebufferCreateInfo create_info { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
    create_info.renderPass = render_pass;
    create_info.attachmentCount = (uint32_t)attachments.size();
    create_info.pAttachments = attachments.data();
    create_info.width = extent.width;
    create_info.height = extent.height;
    create_info.layers = 1;

    VK_CHECK(vkCreateFramebuffer(window_data->context_data->device, &create_info, window_data->context_data->allocator, &framebuffers[i]));
  }
}

void VulkanSwapchain::DestroySwapchainFramebuffers() {
  CALCIUM_PROFILE_FUNCTION();

  for (auto framebuffer : framebuffers) {
    vkDestroyFramebuffer(window_data->context_data->device, framebuffer, window_data->context_data->allocator);
  }
}

void VulkanSwapchain::SetClearValue(const Colour& colour) {
  CALCIUM_PROFILE_FUNCTION();

  clear_values.clear();

  VkClearValue colour_clear_value { };
  colour_clear_value.color.float32[0] = colour.r;
  colour_clear_value.color.float32[1] = colour.g;
  colour_clear_value.color.float32[2] = colour.b;
  colour_clear_value.color.float32[3] = colour.a;
  clear_values.push_back(colour_clear_value);

  if (enable_depth_test) {
    VkClearValue depth_clear_value { };
    depth_clear_value.depthStencil = { 1.0f, 0 };
    clear_values.push_back(depth_clear_value);
  }
}

#pragma warning(pop)

}
