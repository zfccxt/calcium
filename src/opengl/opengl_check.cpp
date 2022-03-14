#include "opengl_check.hpp"

#ifdef CALCIUM_BUILD_DEBUG

#include <assert.h>
#include <cstdio>
#include <string>
#include <vector>

#include <glad/glad.h>

namespace cl::opengl {

static const char* GetGlErrorString(GLenum err) {
  switch(err) {
    case GL_INVALID_ENUM: return "Invalid Enum";
    case GL_INVALID_VALUE: return "Invalid Value";
    case GL_INVALID_OPERATION: return "Invalid Operation";
    case GL_STACK_OVERFLOW: return "Stack Overflow";
    case GL_STACK_UNDERFLOW: return "Stack Underflow";
    case GL_OUT_OF_MEMORY: return "Out of Memory";
    default: return "Unknown Error";
  }
}

void PrintOpenGLError(const char* stmt, const char* fname, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error %08x (%s) at %s:%i - for %s\n", err, GetGlErrorString(err), fname, line, stmt);
    assert(false);
  }
}

}

#endif
