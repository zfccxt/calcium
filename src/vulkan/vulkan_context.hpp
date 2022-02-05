#pragma once

#include "context.hpp"
#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanContext : public Context {
public:
  VulkanContext();
  virtual ~VulkanContext();

  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) override;
  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& create_info) override;

  virtual void BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) override;

private:
  VulkanContextData context_data_;
};

}
