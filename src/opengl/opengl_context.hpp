#pragma once

#include "context.hpp"

namespace cl::OpenGL {

class OpenGLContext : public Context {
public:
  virtual std::unique_ptr<Window> CreateWindow() override;
  virtual std::unique_ptr<Window> CreateWindow(size_t width, size_t height) override;
  virtual std::unique_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) override;
};

}
