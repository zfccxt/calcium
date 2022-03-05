#pragma once

#include <vulkan/vulkan.h>

#include "glfw_window.hpp"
#include "vulkan/vulkan_constants.hpp"
#include "vulkan/vulkan_window_data.hpp"
#include "vulkan/vulkan_shader.hpp"
#include "vulkan/vulkan_swapchain.hpp"

namespace cl::vulkan {

class VulkanWindow : public GlfwWindow {
public:
  VulkanWindow(VulkanContextData* context_data, WindowCreateInfo create_info);
  virtual ~VulkanWindow();

  virtual void SetClearColour(const Colour& colour) override;

public:
  inline VkExtent2D GetFramebufferExtent() const { return window_data_.swapchain.extent; }
  inline VkRenderPass GetRenderPass() const { return window_data_.swapchain.render_pass; }
  inline bool IsDepthTestEnabled() const { return window_data_.swapchain.enable_depth_test; }
  inline bool IsBackfaceCullingEnabled() const { return window_data_.enable_backface_cull; }
  inline WindingOrder GetPolygonFrontFace() const { return window_data_.front_face; }
  inline size_t GetCurrentFrameIndex() const { return window_data_.render_command_buffers.current_command_buffer_index; }
  inline size_t GetNextFrameIndex() const { return (window_data_.render_command_buffers.current_command_buffer_index + 1) % kMaxFramesInFlight; }
  inline VkCommandBuffer GetCurrentRenderCommandBuffer() const { return window_data_.render_command_buffers.current_command_buffer; }
  inline VkPhysicalDevice GetPhysicalDevice() const { return window_data_.context_data->physical_device; }
  inline VkDevice GetDevice() const { return window_data_.context_data->device; }
  inline VkAllocationCallbacks* GetAllocator() const { return window_data_.context_data->allocator; }
  inline VkQueue GetGraphicsQueue() const { return window_data_.context_data->graphics_queue; }
  inline uint32_t GetImageCount() const { return window_data_.swapchain.image_count; }

  uint32_t GetGraphicsQueueFamily() const;
  uint32_t GetMinImageCount() const;

  inline VulkanContextData* GetContextData() const { return window_data_.context_data; }

public:
  void BeginRenderCommandBuffer();
  void EndAndSubmitRenderCommandBuffer();

public:
  inline VkInstance GetInstance() const { return window_data_.context_data->instance; };

protected:
  virtual void OnFramebufferResize(int width, int height) override;

private:
  VulkanWindowData window_data_;
};

}
