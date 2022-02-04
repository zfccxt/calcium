#pragma once

#include <glad/glad.h>

#include "shader.hpp"
#include "shader_create_info.hpp"

namespace cl::OpenGL {

class OpenGLShader : public Shader {
public:
  OpenGLShader(const ShaderCreateInfo& shader_info);
  virtual ~OpenGLShader();

  virtual void Bind() override;

private:
  GLuint program_id_;
};

}
