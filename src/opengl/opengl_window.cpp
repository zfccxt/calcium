#include "opengl_window.hpp"

#include <GLFW/glfw3.h>

namespace cl::OpenGL {

OpenGLWindow::OpenGLWindow(const WindowCreateInfo& create_info) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  // TODO: Figure out highest supported OpenGL version on the current machine
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef CALCIUM_PLATFORM_MACOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  CreateGlfwWindow(create_info);
}

void OpenGLWindow::SwapBuffers() {
  glfwSwapBuffers(glfw_window_);
}

}
