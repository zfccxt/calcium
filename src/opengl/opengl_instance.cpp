#include "opengl_instance.hpp"

#include "opengl/opengl_window.hpp"

namespace cl::OpenGL {

std::unique_ptr<Window> OpenGLInstance::CreateWindow() const {
  WindowCreateInfo create_info;
  return std::make_unique<OpenGLWindow>(create_info);
}

std::unique_ptr<Window> OpenGLInstance::CreateWindow(size_t width, size_t height) const {
  WindowCreateInfo create_info;
  create_info.width = width;
  create_info.height = height;
  return std::make_unique<OpenGLWindow>(create_info);
}

std::unique_ptr<Window> OpenGLInstance::CreateWindow(const WindowCreateInfo& create_info) const {
  return std::make_unique<OpenGLWindow>(create_info);
}

}
