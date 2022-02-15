#pragma once

#include <memory>

#include <glad/glad.h>

#include "opengl_uniform_buffer.hpp"
#include "shader.hpp"
#include "shader_create_info.hpp"

namespace cl::OpenGL {

class OpenGLShader : public Shader {
public:
  OpenGLShader(const ShaderCreateInfo& shader_info);
  virtual ~OpenGLShader();

  virtual void Bind() override;

  virtual void UploadUniform(int binding, void* data) override;

private:
  GLuint program_id_;

  std::vector<std::unique_ptr<OpenGLUniformBuffer>> uniforms;
};

}
