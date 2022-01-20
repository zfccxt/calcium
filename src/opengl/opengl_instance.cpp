#include "opengl_instance.hpp"

#include "opengl/opengl_window.hpp"

namespace cl::OpenGL {

std::unique_ptr<Window> OpenGLInstance::CreateWindow(const WindowCreateInfo& create_info) const {
  return std::make_unique<OpenGLWindow>(create_info);
}

}
