#pragma once

#include <memory>

#include "context.hpp"
#include "opengl/opengl_texture.hpp"
#include "opengl/opengl_window.hpp"

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
  
  virtual void BeginRenderPass(const std::shared_ptr<Shader>& shader) override;
  virtual void EndRenderPass() override;

  virtual std::shared_ptr<Texture> GetBlankTexture() override;

private:
  // TODO: We only support windows as render targets for now, so this is a pointer to an OpenGLWindow when it should
  // really be a pointer to a RenderTarget
  std::weak_ptr<OpenGLWindow> bound_render_target_;

  std::shared_ptr<OpenGLTexture> blank_texture_;
};

}
