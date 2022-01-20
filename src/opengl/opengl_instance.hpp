#pragma once

#include "instance.hpp"

namespace cl::OpenGL {

class OpenGLInstance : public Instance {
public:
  virtual std::unique_ptr<Window> CreateWindow() const override;
};

}
