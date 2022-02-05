#include "opengl_context.hpp"

#include "glfw_utils.hpp"
#include "opengl/opengl_shader.hpp"
#include "opengl/opengl_window.hpp"

namespace cl::OpenGL {

OpenGLContext::OpenGLContext() {
  GLFW::IncrementGLFWContextCount();
}

OpenGLContext::~OpenGLContext() {
  GLFW::DecrementGLFWContextCount();
}

std::shared_ptr<Window> OpenGLContext::CreateWindow(const WindowCreateInfo& create_info) {
  return std::make_shared<OpenGLWindow>(create_info);
}

std::shared_ptr<Shader> OpenGLContext::CreateShader(const ShaderCreateInfo& create_info) {
  return std::make_shared<OpenGLShader>(create_info);
}

void OpenGLContext::BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) {
  // TODO: if window, make context current otherwise, bind framebuffer as render target
}

}
