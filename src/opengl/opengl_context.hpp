#pragma once

#include <memory>

#include "context.hpp"
#include "opengl/opengl_texture.hpp"
#include "opengl/opengl_window.hpp"

namespace cl::opengl {

class OpenGLContext : public Context {
public:
  OpenGLContext();
  ~OpenGLContext();

  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& window_info) override;
  virtual std::shared_ptr<Mesh> CreateMesh(const MeshCreateInfo& mesh_info) override;
  virtual std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassCreateInfo&) override;
  virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& texture_info) override;

  virtual void BeginFrame() override;
  virtual void EndFrame() override;

public:
  std::shared_ptr<OpenGLTexture> GetBlankTexture() const;

private:
  std::shared_ptr<OpenGLTexture> blank_texture_;
};

}
