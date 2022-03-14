#pragma once

#ifdef CALCIUM_BUILD_DEBUG
  namespace cl::opengl {
    void PrintOpenGLError(const char* stmt, const char* fname, int line);
  }

  #define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
          } while (0)
#else
  #define GL_CHECK(stmt) stmt
#endif

