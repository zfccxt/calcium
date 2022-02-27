#pragma once

#include <cstdint>

#include "glfw_window.hpp"

namespace cl::OpenGL {

class OpenGLWindow : public GlfwWindow {
public:
  OpenGLWindow(WindowCreateInfo create_info);

  virtual void SetClearColour(const Colour& colour) override;

public:
  void Clear();
  void SwapBuffers();

  void MakeContextCurrent();

protected:
  virtual void OnFramebufferResize(int width, int height) override;

private:
  uint32_t gl_clear_bits_;
};

}
