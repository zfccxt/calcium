#include "opengl_shader.hpp"

namespace cl::OpenGL {

OpenGLShader::OpenGLShader(const ShaderCreateInfo& shader_info) {
  // TODO
}

OpenGLShader::~OpenGLShader() {
  glDeleteProgram(program_id_);
}

void OpenGLShader::Bind() {
  glUseProgram(program_id_);
}

}
