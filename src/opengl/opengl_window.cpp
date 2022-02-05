#include "opengl_window.hpp"

#include <GLFW/glfw3.h>

namespace cl::OpenGL {

OpenGLWindow::OpenGLWindow(WindowCreateInfo create_info) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  // TODO: Figure out highest supported OpenGL version on the current machine
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef CALCIUM_PLATFORM_MACOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  if (create_info.title.empty()) {
    create_info.title = "Calcium [OpenGL]";
#ifdef CALCIUM_BUILD_DEBUG
    create_info.title += " [Debug Build]";
#endif
  }

  CreateGlfwWindow(create_info);

  // Set the viewport to the size of the whole window framebuffer
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  glViewport(0, 0, width, height);
}

void OpenGLWindow::SwapBuffers() {
  glfwSwapBuffers(glfw_window_);
}

void OpenGLWindow::MakeContextCurrent() {
  glfwMakeContextCurrent(glfw_window_);
}

}
