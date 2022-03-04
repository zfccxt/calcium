#pragma once

#include <string>

#include <glad/glad.h>

#include "shader_stage.hpp"

namespace cl::opengl {

GLenum GetOpenGLShaderType(ShaderStage type);
GLuint CreateShaderModule(const std::string& glsl_code, ShaderStage shader_type);

}
