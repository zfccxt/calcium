#pragma once

#include "context.hpp"

namespace cl::OpenGL {

class OpenGLContext : public Context {
public:
  OpenGLContext();
  ~OpenGLContext();

  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) override;
  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& create_info) override;

  virtual void BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) override;
};

}
