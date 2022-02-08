#include "opengl_context.hpp"

#include "glfw_utils.hpp"
#include "opengl/opengl_mesh.hpp"
#include "opengl/opengl_shader.hpp"
#include "opengl/opengl_window.hpp"

namespace cl::OpenGL {

OpenGLContext::OpenGLContext() {
  GLFW::IncrementGLFWContextCount();
}

OpenGLContext::~OpenGLContext() {
  GLFW::DecrementGLFWContextCount();
}

std::shared_ptr<Window> OpenGLContext::CreateWindow(const WindowCreateInfo& window_info) {
  return std::make_shared<OpenGLWindow>(window_info);
}

std::shared_ptr<Shader> OpenGLContext::CreateShader(const ShaderCreateInfo& shader_info) {
  return std::make_shared<OpenGLShader>(shader_info);
}

std::shared_ptr<Mesh> OpenGLContext::CreateMesh(const MeshCreateInfo& mesh_info) {
  return std::make_shared<OpenGLMesh>(mesh_info);
}

void OpenGLContext::BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) {
  // TODO: figure out whether this is a window or a framebuffer and bind appropriately

  // If render target is a window:
  auto opengl_window = (OpenGLWindow*)(render_target.get());
  opengl_window->MakeContextCurrent();
}

}
