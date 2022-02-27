#include "opengl_window.hpp"

#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cl::OpenGL {

const static int kSuitableOpenGLVersions[8][2] = { {4,6}, {4,5}, {4,4}, {4,3}, {4,2}, {4,1}, {4,0}, {3,3} };

OpenGLWindow::OpenGLWindow(WindowCreateInfo create_info) {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  // Use a ridiculous hack to guarantee the highest possible OpenGL context version supported on the current machine
  
  // There is no clean way to request the highest possible OpenGL context version that the current machine supports
  // GLFW will _attempt_ to provide this if you don't specify a context version, but this frequently does not work.
  
  // When we use glfwWindowHint to request a context version, the operating system is allowed to create a context of any
  // version compatible with the one requested. Therefore if we request version 3.3 at minimum hoping to recieve a 4.6
  // context, we may very well get a 3.3 context. This is unacceptable for this application, since we want SPIRV-Cross to
  // have access to as many up-to-date GLSL features as possible. We cannot tell ahead of time what OpenGL features we
  // will need - these are inferred at runtime by cross-compiling SPIR-V shaders.
  
  // We keep a list of suitable OpenGL versions is order of priority and loop through them trying to create contexts.
  // As soon as context creation succeeds, we have found the OpenGL version to use, and we continue with the program.
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  for (size_t i = 0; i < sizeof(kSuitableOpenGLVersions) / sizeof(kSuitableOpenGLVersions[0]); ++i) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kSuitableOpenGLVersions[i][0]);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kSuitableOpenGLVersions[i][1]);
    GLFWwindow* offscreen_context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (offscreen_context) {
      glfwDestroyWindow(offscreen_context);
      break;
    }
  }
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

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

  // Make context current so that we can call OpenGL functions on the window's context
  MakeContextCurrent();

  // Load OpenGL functions - this requires a context, so can't be done before window creation
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    assert(false);
  }

  // Set the viewport to the size of the whole window framebuffer
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  glViewport(0, 0, width, height);

  // Set vsync: 0 is unlimited framerate, 1 is default swap interval
  glfwSwapInterval(create_info.enable_vsync ? 1 : 0);

  // Enable or disable depth test
  if (create_info.enable_depth_test) {
    glEnable(GL_DEPTH_TEST);
    gl_clear_bits_ = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  }
  else {
    gl_clear_bits_ = GL_COLOR_BUFFER_BIT;
  }
  SetClearColour(create_info.clear_colour);

  // Enable or disable backface culling
  if (create_info.enable_backface_cull) {
    glFrontFace(create_info.front_face == WindingOrder::kClockwise ? GL_CW : GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
}

void OpenGLWindow::Clear() {
  glClear(gl_clear_bits_);
}

void OpenGLWindow::SwapBuffers() {
  glfwSwapBuffers(glfw_window_);
}

void OpenGLWindow::SetClearColour(const Colour& colour) {
  glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void OpenGLWindow::MakeContextCurrent() {
  glfwMakeContextCurrent(glfw_window_);
}

void OpenGLWindow::OnFramebufferResize(int width, int height) {
  glViewport(0, 0, width, height);
}

}
