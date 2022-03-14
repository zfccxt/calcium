#include "opengl_error.hpp"

#include <cstdio>

namespace cl::opengl {

void GLAPIENTRY GlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param) {
  fprintf(stderr, "[OpenGL] %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

}
