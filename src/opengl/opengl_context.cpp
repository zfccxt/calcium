#include "opengl_context.hpp"

#include "glfw_utils.hpp"
#include "instrumentor.hpp"
#include "opengl/opengl_check.hpp"
#include "opengl/opengl_error.hpp"
#include "opengl/opengl_mesh.hpp"
#include "opengl/opengl_shader.hpp"
#include "opengl/opengl_texture.hpp"
#include "opengl/opengl_texture_array.hpp"
#include "opengl/opengl_window.hpp"

namespace cl::opengl {

OpenGLContext::OpenGLContext() {
  glfw::GlfwInit();
}

OpenGLContext::~OpenGLContext() {
  ClxOnDestroy();
}

std::shared_ptr<Window> OpenGLContext::CreateWindow(const WindowCreateInfo& window_info) {
  CALCIUM_PROFILE_FUNCTION();

  // For convenience we automatically bind the first window the library user creates as the current render target
  auto window = std::make_shared<OpenGLWindow>(window_info);
  if (!bound_render_target_.lock()) {
    BindRendertarget(window);

#ifdef CALCIUM_BUILD_DEBUG
    GL_CHECK(glEnable(GL_DEBUG_OUTPUT));
    GL_CHECK(glDebugMessageCallback(GlErrorCallback, 0));
#endif
  }

  // TODO: We should really create the blank texture on context creation, but we have to wait until there is a valid
  // OpenGL context, which requires waiting for a window to be created
  if (!blank_texture_) {
    BlankTextureCreateInfo texture_info;
    blank_texture_ = std::make_shared<OpenGLTexture>(texture_info);
  }
  if (!blank_texture_array_) {
    BlankTextureCreateInfo texture_info;
    blank_texture_array_ = std::make_shared<OpenGLTextureArray>(texture_info);
  }

  return window;
}

std::shared_ptr<Shader> OpenGLContext::CreateShader(const ShaderCreateInfo& shader_info) {
  CALCIUM_PROFILE_FUNCTION();

  auto shader = std::make_shared<OpenGLShader>(shader_info);
  shader->Bind();
  shader->BindAllTextureSamplers(blank_texture_);
  shader->BindAllTextureArraySamplers(blank_texture_array_);
  return shader;
}

std::shared_ptr<Mesh> OpenGLContext::CreateMesh(const MeshCreateInfo& mesh_info) {
  CALCIUM_PROFILE_FUNCTION();

  return std::make_shared<OpenGLMesh>(mesh_info);
}

std::shared_ptr<Texture> OpenGLContext::CreateTexture(const TextureCreateInfo& texture_info) {
  CALCIUM_PROFILE_FUNCTION();

  return std::make_shared<OpenGLTexture>(texture_info);
}

std::shared_ptr<Texture> OpenGLContext::CreateTexture(const BlankTextureCreateInfo& texture_info) {
  CALCIUM_PROFILE_FUNCTION();

  return std::make_shared<OpenGLTexture>(texture_info);
}

std::shared_ptr<TextureArray> OpenGLContext::CreateTextureArray(const TextureArrayCreateInfo& texture_array_info) {
  CALCIUM_PROFILE_FUNCTION();

  return std::make_shared<OpenGLTextureArray>(texture_array_info);
}

void OpenGLContext::BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) {
  CALCIUM_PROFILE_FUNCTION();

  bound_render_target_ = std::dynamic_pointer_cast<OpenGLWindow>(render_target);

  // TODO: figure out whether this is a window or a framebuffer and bind appropriately
  // If render target is a window:
  auto opengl_window = (OpenGLWindow*)(render_target.get());
  opengl_window->MakeContextCurrent();

  ClxOnBindRenderTarget(render_target);
}

void OpenGLContext::BeginFrame() {
  CALCIUM_PROFILE_FUNCTION();

  // TODO: Make this work with bound framebuffers

  // If the bound render target is a window:
  auto window = bound_render_target_.lock();
  window->Clear();
}

void OpenGLContext::EndFrame() {
  CALCIUM_PROFILE_FUNCTION();

  auto window = bound_render_target_.lock();
  // TODO: Make this work with bound framebuffers

  // If the bound render target is a window:
  window->SwapBuffers();
}

}
