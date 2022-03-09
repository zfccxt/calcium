#pragma once

#include "context.hpp"
#include "vulkan/vulkan_context_data.hpp"
#include "vulkan/vulkan_window.hpp"

namespace cl::vulkan {

class VulkanContext : public Context {
public:
  VulkanContext();
  virtual ~VulkanContext();

  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& window_info) override;
  virtual std::shared_ptr<Mesh> CreateMesh(const MeshCreateInfo& mesh_info) override;
  virtual std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassCreateInfo& render_pass_info) override;
  virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& texture_info) override;

  virtual void BeginFrame() override;
  virtual void EndFrame() override;

private:
  VulkanContextData context_data_;
};

}
