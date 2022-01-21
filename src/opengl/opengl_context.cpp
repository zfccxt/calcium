#include "opengl_context.hpp"

#include "opengl/opengl_window.hpp"

namespace cl::OpenGL {

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

}
