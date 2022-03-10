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

  virtual void SetDepthTestEnable(bool enable) override;
  virtual bool IsDepthTestEnabled() const override;

  uint32_t GetGraphicsQueueFamily() const;
  uint32_t GetMinImageCount() const;

  inline VulkanContextData* GetContextData() const { return window_data_.context_data; }
  VulkanSwapchain* GetSwapchain() { return &(window_data_.swapchain); };

public:
  VkExtent2D GetFramebufferExtent() const;
  VkRenderPass GetRenderPass() const;
  bool IsBackfaceCullingEnabled() const;
  WindingOrder GetPolygonFrontFace() const;
  size_t GetCurrentFrameIndex() const;
  size_t GetNextFrameIndex() const;
  VkCommandBuffer GetCurrentRenderCommandBuffer() const;
  VkPhysicalDevice GetPhysicalDevice() const;
  VkDevice GetDevice() const;
  VkAllocationCallbacks* GetAllocator() const;
  VkQueue GetGraphicsQueue() const;
  uint32_t GetImageCount() const;
  VkFormat GetSurfaceFormat() const;

public:
  void BeginRenderCommandBuffer();
  void EndAndSubmitRenderCommandBuffer();

public:
  inline VkInstance GetInstance() const { return window_data_.context_data->instance; };

protected:
  virtual void OnFramebufferResize(int width, int height) override;

private:
  VulkanWindowData window_data_;

  PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT_;
};

}
