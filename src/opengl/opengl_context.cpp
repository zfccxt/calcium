#include "opengl_context.hpp"

#include "glfw_utils.hpp"
#include "opengl/opengl_mesh.hpp"
#include "opengl/opengl_shader.hpp"
#include "opengl/opengl_texture.hpp"
#include "opengl/opengl_window.hpp"

namespace cl::OpenGL {

OpenGLContext::OpenGLContext() {
  GLFW::IncrementGLFWContextCount();
}

OpenGLContext::~OpenGLContext() {
  GLFW::DecrementGLFWContextCount();
}

std::shared_ptr<Window> OpenGLContext::CreateWindow(const WindowCreateInfo& window_info) {
  // For convenience we automatically bind the first window the library user creates as the current render target
  auto window = std::make_shared<OpenGLWindow>(window_info);
  bound_render_target_ = window;

  // TODO: We should really create the blank texture on context creation, but we have to wait until there is a valid
  // OpenGL context, which requires waiting for a window to be created
  if (!blank_texture_) {
    BlankTextureCreateInfo texture_info;
    blank_texture_ = std::make_shared<OpenGLTexture>(texture_info);
  }

  return window;
}

std::shared_ptr<Shader> OpenGLContext::CreateShader(const ShaderCreateInfo& shader_info) {
  return std::make_shared<OpenGLShader>(shader_info);
}

std::shared_ptr<Mesh> OpenGLContext::CreateMesh(const MeshCreateInfo& mesh_info) {
  return std::make_shared<OpenGLMesh>(mesh_info);
}

std::shared_ptr<Texture> OpenGLContext::CreateTexture(const TextureCreateInfo& texture_info) {
  return std::make_shared<OpenGLTexture>(texture_info);
}

void OpenGLContext::BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) {
  // TODO: figure out whether this is a window or a framebuffer and bind appropriately

  // If render target is a window:
  auto opengl_window = (OpenGLWindow*)(render_target.get());
  opengl_window->MakeContextCurrent();
}

void OpenGLContext::BeginRenderPass(const std::shared_ptr<Shader>& shader) {
  auto window = bound_render_target_.lock();
  // TODO: Make this work with bound framebuffers

  // If the bound render target is a window:
  window->Clear();
  
  auto s = std::dynamic_pointer_cast<OpenGLShader>(shader);
  s->Bind();
}

void OpenGLContext::EndRenderPass() {
  auto window = bound_render_target_.lock();
  // TODO: Make this work with bound framebuffers

  // If the bound render target is a window:
  window->SwapBuffers();
}

std::shared_ptr<Texture> OpenGLContext::GetBlankTexture() {
  return blank_texture_;
}

}
