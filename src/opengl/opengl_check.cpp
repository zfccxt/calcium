#include "opengl_check.hpp"

#ifdef CALCIUM_BUILD_DEBUG

#include <assert.h>
#include <cstdio>

#include <glad/glad.h>

namespace cl::opengl {

void PrintOpenGLError(const char* stmt, const char* fname, int line) {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
      fprintf(stderr, "OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
      assert(false);
  }
}

}

#endif
