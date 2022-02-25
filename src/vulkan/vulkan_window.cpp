#include "vulkan_window.hpp"

#include <GLFW/glfw3.h>

#include "vulkan/vulkan_swapchain_render_pass.hpp"
#include "vulkan/vulkan_window_surface.hpp"

namespace cl::Vulkan {

VulkanWindow::VulkanWindow(VulkanContextData* context_data, WindowCreateInfo create_info) {
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
  window_data_.render_command_buffers.DestroyRenderCommandBuffers(window_data_);

  window_data_.swapchain.sync_objects.DestroySwapchainSyncObjects(window_data_);
  window_data_.swapchain.DestroySwapchainFramebuffers();
  DestroySwapchainRenderPass(window_data_.swapchain, window_data_.swapchain.render_pass);
  window_data_.swapchain.DestroySwapchain();

  DestroyWindowSurface(window_data_, window_data_.surface);
}

void VulkanWindow::SetClearColour(const Colour& colour) {
  // TODO
}

VkExtent2D VulkanWindow::GetFramebufferExtent() {
  return window_data_.swapchain.extent;
}

VkRenderPass VulkanWindow::GetRenderPass() {
  return window_data_.swapchain.render_pass;
}

bool VulkanWindow::IsDepthTestEnabled() const {
  return window_data_.swapchain.enable_depth_test;
}

bool VulkanWindow::IsBackfaceCullingEnabled() const {
  return window_data_.enable_backface_cull;
}

WindingOrder VulkanWindow::GetPolygonFrontFace() const {
  return window_data_.front_face;
}

size_t VulkanWindow::GetCurrentFrameIndex() const {
  return window_data_.render_command_buffers.current_command_buffer_index;
}

void VulkanWindow::BeginRenderCommandBuffer(const std::shared_ptr<VulkanShader>& shader) {
  window_data_.render_command_buffers.BeginRenderCommandBuffer(window_data_);

  shader->Bind(window_data_.render_command_buffers.current_command_buffer);
}

void VulkanWindow::EndAndSubmitRenderCommandBuffer() {
  window_data_.render_command_buffers.EndAndSubmitRenderCommandBuffer(window_data_);
}

VkCommandBuffer VulkanWindow::GetCurrentRenderCommandBuffer() {
  return window_data_.render_command_buffers.current_command_buffer;
}

}
