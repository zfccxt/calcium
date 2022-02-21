#pragma once

#include "context.hpp"
#include "vulkan/vulkan_context_data.hpp"
#include "vulkan/vulkan_window.hpp"

namespace cl::Vulkan {

class VulkanContext : public Context {
public:
  VulkanContext();
  virtual ~VulkanContext();

  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& window_info) override;
  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& shader_info) override;
  virtual std::shared_ptr<Mesh> CreateMesh(const MeshCreateInfo& mesh_info) override;
  virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& texture_info) override;

  virtual void BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) override;

private:
  VulkanContextData context_data_;

  // We only support windows as render targets right now, so this is a pointer to a VulkanWindow
  std::weak_ptr<VulkanWindow> bound_render_target_;
};

}
