#pragma once

#include "shader.hpp"
#include "shader_create_info.hpp"

namespace cl::OpenGL {

class OpenGLShader : public Shader {
public:
  OpenGLShader(const ShaderCreateInfo& shader_info);

  virtual void Bind() override;
};

}
