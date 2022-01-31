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

std::unique_ptr<Window> OpenGLContext::CreateWindow() {
  WindowCreateInfo create_info;
  return std::make_unique<OpenGLWindow>(create_info);
}

std::unique_ptr<Window> OpenGLContext::CreateWindow(size_t width, size_t height) {
  WindowCreateInfo create_info;
  create_info.width = width;
  create_info.height = height;
  return std::make_unique<OpenGLWindow>(create_info);
}

std::unique_ptr<Window> OpenGLContext::CreateWindow(const WindowCreateInfo& create_info) {
  return std::make_unique<OpenGLWindow>(create_info);
}

std::unique_ptr<Shader> OpenGLContext::CreateShader(const std::string& vert_path, const std::string& frag_path) {
  ShaderCreateInfo shader_info;
  shader_info.vert_path = vert_path;
  shader_info.frag_path = frag_path;
  return std::make_unique<OpenGLShader>(shader_info);
}

std::unique_ptr<Shader> OpenGLContext::CreateShader(const ShaderCreateInfo& create_info) {
  return std::make_unique<OpenGLShader>(create_info);
}

}
