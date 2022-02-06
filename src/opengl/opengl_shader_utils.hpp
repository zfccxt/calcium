#pragma once

#include <string>

#include <glad/glad.h>

#include "shader_type.hpp"

namespace cl::OpenGL {

GLenum GetOpenGLShaderType(ShaderType type);
GLuint CreateShaderModule(const std::string& glsl_code, ShaderType shader_type);

}
