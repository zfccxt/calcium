#pragma once

#include <vulkan/vulkan.h>

#include "glfw_window.hpp"
#include "vulkan/vulkan_window_data.hpp"
#include "vulkan/vulkan_shader.hpp"
#include "vulkan/vulkan_swapchain.hpp"

namespace cl::Vulkan {

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

  void BeginRenderCommandBuffer(const std::shared_ptr<VulkanShader>& shader);
  void EndAndSubmitRenderCommandBuffer();
  VkCommandBuffer GetCurrentRenderCommandBuffer();

private:
  VulkanWindowData window_data_;
};

}
