#pragma once

#include <cstdint>

#include "glfw_window.hpp"

namespace cl::OpenGL {

class OpenGLWindow : public GlfwWindow {
public:
  OpenGLWindow(WindowCreateInfo create_info);

  virtual void Clear() override;
  virtual void SwapBuffers() override;

  virtual void SetClearColour(const Colour& colour) override;

  void MakeContextCurrent();

private:
  uint32_t gl_clear_bits_;
};

}
