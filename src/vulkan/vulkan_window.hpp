#pragma once

#include <vulkan/vulkan.h>

#include "glfw_window.hpp"
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
  VkExtent2D GetFramebufferExtent();
  VkRenderPass GetRenderPass();
  bool IsDepthTestEnabled() const;
  bool IsBackfaceCullingEnabled() const;
  WindingOrder GetPolygonFrontFace() const;
  size_t GetCurrentFrameIndex() const;
  size_t GetNextFrameIndex() const;

  void BeginRenderCommandBuffer(const std::shared_ptr<VulkanShader>& shader);
  void EndAndSubmitRenderCommandBuffer();
  VkCommandBuffer GetCurrentRenderCommandBuffer();

protected:
  virtual void OnFramebufferResize(int width, int height) override;

private:
  VulkanWindowData window_data_;
};

}
