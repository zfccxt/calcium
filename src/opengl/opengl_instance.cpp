#include "opengl_instance.hpp"

#include "opengl/opengl_window.hpp"

namespace cl::OpenGL {

std::unique_ptr<Window> OpenGLInstance::CreateWindow() const {
  return std::unique_ptr<OpenGLWindow>();
}

}
