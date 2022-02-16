#pragma once

#include "context.hpp"

namespace cl::OpenGL {

class OpenGLContext : public Context {
public:
  OpenGLContext();
  ~OpenGLContext();

  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& window_info) override;
  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& shader_info) override;
  virtual std::shared_ptr<Mesh> CreateMesh(const MeshCreateInfo& mesh_info) override;
  virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& texture_info) override;

  virtual void BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) override;
};

}
