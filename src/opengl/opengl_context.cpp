#include "opengl_context.hpp"

#include "glfw_utils.hpp"
#include "instrumentor.hpp"
#include "opengl/opengl_mesh.hpp"
#include "opengl/opengl_render_pass.hpp"
#include "opengl/opengl_texture.hpp"
#include "opengl/opengl_window.hpp"

namespace cl::opengl {

OpenGLContext::OpenGLContext() {
  glfw::IncrementGLFWContextCount();
}

OpenGLContext::~OpenGLContext() {
  ClxOnDestroy();

  glfw::DecrementGLFWContextCount();
}

std::shared_ptr<Window> OpenGLContext::CreateWindow(const WindowCreateInfo& window_info) {
  CALCIUM_PROFILE_FUNCTION();

  // We require a context to create the texture
  auto window = std::make_shared<OpenGLWindow>(window_info, this);

  // TODO: We should really create the blank texture on context creation, but we have to wait until there is a valid
  // OpenGL context, which requires waiting for a window to be created
  // OpenGL is silly
  // I think the normal way to get around this is to create an invisible window
  if (!blank_texture_) {
    BlankTextureCreateInfo texture_info;
    blank_texture_ = std::make_shared<OpenGLTexture>(texture_info);
  }

  return window;
}

std::shared_ptr<Mesh> OpenGLContext::CreateMesh(const MeshCreateInfo& mesh_info) {
  CALCIUM_PROFILE_FUNCTION();

  return std::make_shared<OpenGLMesh>(mesh_info);
}

std::shared_ptr<RenderPass> OpenGLContext::CreateRenderPass(const RenderPassCreateInfo& render_pass_info) {
  CALCIUM_PROFILE_FUNCTION();

  return std::make_shared<OpenGLRenderPass>(render_pass_info);
}

std::shared_ptr<Texture> OpenGLContext::CreateTexture(const TextureCreateInfo& texture_info) {
  CALCIUM_PROFILE_FUNCTION();

  return std::make_shared<OpenGLTexture>(texture_info);
}

std::shared_ptr<OpenGLTexture> OpenGLContext::GetBlankTexture() const {
  return blank_texture_;
}

void OpenGLContext::BeginFrame() { /* Intentionally empty */ }

void OpenGLContext::EndFrame()   { /* Intentionally empty */ }

}
