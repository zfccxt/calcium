#include "vulkan_window.hpp"

#include <GLFW/glfw3.h>

#include "instrumentor.hpp"
#include "vulkan/vulkan_image_utils.hpp"
#include "vulkan/vulkan_render_pass.hpp"
#include "vulkan/vulkan_swapchain_support_details.hpp"
#include "vulkan/vulkan_queue_family_indices.hpp"
#include "vulkan/vulkan_window_surface.hpp"

namespace cl::vulkan {

VulkanWindow::VulkanWindow(VulkanContextData* context_data, WindowCreateInfo create_info) {
  CALCIUM_PROFILE_FUNCTION();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  if (create_info.title.empty()) {
    create_info.title = "Calcium [Vulkan]";
#ifdef CALCIUM_BUILD_DEBUG
    create_info.title += " [Debug Build]";
#endif
  }
  CreateGlfwWindow(create_info);

  window_data_.context_data = context_data;
  window_data_.glfw_window = glfw_window_;
  window_data_.enable_vsync = create_info.enable_vsync;
  window_data_.enable_depth_test = create_info.enable_depth_test;
  window_data_.enable_backface_cull = create_info.enable_backface_cull;
  window_data_.front_face = create_info.front_face;

  window_data_.surface = CreateWindowSurface(window_data_);

  window_data_.swapchain.window_data = &window_data_;
  window_data_.swapchain.enable_depth_test = create_info.enable_depth_test;
  window_data_.swapchain.CreateSwapchain();
  window_data_.swapchain.SetClearValue(create_info.clear_colour);
  window_data_.swapchain.render_pass = CreateSwapchainRenderPass(window_data_.swapchain);
  window_data_.swapchain.CreateSwapchainFramebuffers();
  window_data_.swapchain.sync_objects.CreateSwapchainSyncObjects(window_data_);
  
  window_data_.render_command_buffers.CreateRenderCommandBuffers(window_data_);
}

VulkanWindow::~VulkanWindow() {
  CALCIUM_PROFILE_FUNCTION();

  window_data_.render_command_buffers.DestroyRenderCommandBuffers(window_data_);

  window_data_.swapchain.sync_objects.DestroySwapchainSyncObjects(window_data_);
  window_data_.swapchain.DestroySwapchainFramebuffers();
  vkDestroyRenderPass(window_data_.context_data->device, window_data_.swapchain.render_pass, window_data_.context_data->allocator);
  window_data_.swapchain.DestroySwapchain();

  DestroyWindowSurface(window_data_, window_data_.surface);
}

void VulkanWindow::SetClearColour(const Colour& colour) {
  // TODO
}

uint32_t VulkanWindow::GetGraphicsQueueFamily() const {
  VulkanQueueFamilyIndices queue_families(window_data_.context_data->physical_device, window_data_.surface);
  return queue_families.graphics_family;
}

uint32_t VulkanWindow::GetMinImageCount() const {
  VulkanSwapchainSupportDetails swapchain_support(window_data_.context_data->physical_device, window_data_.surface);
  return swapchain_support.surface_capabilities.minImageCount;
}

void VulkanWindow::BeginRenderCommandBuffer() {
  window_data_.render_command_buffers.BeginRenderCommandBuffer(window_data_);
}

void VulkanWindow::EndAndSubmitRenderCommandBuffer() {
  window_data_.render_command_buffers.EndAndSubmitRenderCommandBuffer(window_data_);
}

void VulkanWindow::OnFramebufferResize(int width, int height) {
  CALCIUM_PROFILE_FUNCTION();

  window_data_.swapchain.RecreateSwapchain();

  std::vector<std::weak_ptr<VulkanShader>>::iterator iter;
  for (iter = window_data_.context_data->loaded_shaders_.begin(); iter != window_data_.context_data->loaded_shaders_.end();) {
    auto shader = iter->lock();

    if (!shader) {
      // If the shader has been destroyed by the client application, delete it from the list of weak pointers to loaded shaders
      iter = window_data_.context_data->loaded_shaders_.erase(iter);
    }
    else {
      // If the shader is still valid, recreate it
      shader->Recreate(window_data_.swapchain.extent, window_data_.swapchain.render_pass);
      ++iter;
    }
  }
}

VkExtent2D VulkanWindow::GetFramebufferExtent() const { return window_data_.swapchain.extent; }
VkRenderPass VulkanWindow::GetRenderPass() const { return window_data_.swapchain.render_pass; }
bool VulkanWindow::IsDepthTestEnabled() const { return window_data_.swapchain.enable_depth_test; }
bool VulkanWindow::IsBackfaceCullingEnabled() const { return window_data_.enable_backface_cull; }
WindingOrder VulkanWindow::GetPolygonFrontFace() const { return window_data_.front_face; }
size_t VulkanWindow::GetCurrentFrameIndex() const { return window_data_.render_command_buffers.current_command_buffer_index; }
size_t VulkanWindow::GetNextFrameIndex() const { return (window_data_.render_command_buffers.current_command_buffer_index + 1) % kMaxFramesInFlight; }
VkCommandBuffer VulkanWindow::GetCurrentRenderCommandBuffer() const { return window_data_.render_command_buffers.current_command_buffer; }
VkPhysicalDevice VulkanWindow::GetPhysicalDevice() const { return window_data_.context_data->physical_device; }
VkDevice VulkanWindow::GetDevice() const { return window_data_.context_data->device; }
VkAllocationCallbacks* VulkanWindow::GetAllocator() const { return window_data_.context_data->allocator; }
VkQueue VulkanWindow::GetGraphicsQueue() const { return window_data_.context_data->graphics_queue; }
uint32_t VulkanWindow::GetImageCount() const { return window_data_.swapchain.image_count; }
VkFormat VulkanWindow::GetSurfaceFormat() const { return window_data_.swapchain.image_format; }

}
