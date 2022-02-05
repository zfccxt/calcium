#pragma once

#include "glfw_window.hpp"

namespace cl::OpenGL {

class OpenGLWindow : public GlfwWindow {
public:
  OpenGLWindow(WindowCreateInfo create_info);

  virtual void SwapBuffers() override;

  void MakeContextCurrent();
};

}
