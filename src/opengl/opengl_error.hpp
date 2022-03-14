#pragma once

#include <glad/glad.h>

namespace cl::opengl {

void GLAPIENTRY GlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

}
