#pragma once

#include <cstdint>

#include "glfw_window.hpp"

namespace cl::opengl {

class OpenGLContext;

class OpenGLWindow : public GlfwWindow {
public:
  OpenGLWindow(WindowCreateInfo create_info, OpenGLContext* context);

  virtual void SetClearColour(const Colour& colour) override;
  virtual void SwapBuffers() override;

public:
  void Clear(unsigned int clear_bits);

  void MakeContextCurrent();

  OpenGLContext* GetContext() const;

protected:
  virtual void OnFramebufferResize(int width, int height) override;

private:
  uint32_t gl_clear_bits_;
  OpenGLContext* context_;
};

}
