#include "glfw_window.hpp"

#include <assert.h>

#include <GLFW/glfw3.h>

#include "glfw_utils.hpp"

namespace cl {

size_t GlfwWindow::num_glfw_windows_ = 0;


GlfwWindow::GlfwWindow() {
  ++num_glfw_windows_;
}

GlfwWindow::~GlfwWindow() {
  --num_glfw_windows_;
}

void GlfwWindow::CreateGlfwWindow(const WindowCreateInfo& create_info) {
  glfw_window_ = glfwCreateWindow(create_info.width, create_info.height, create_info.title.c_str(), nullptr, nullptr);
  assert(glfw_window_);

  if (create_info.center) {
#ifdef CALCIUM_PLATFORM_MACOS
    Center(true, false);
#else
    Center();
#endif
  }
}

bool GlfwWindow::IsOpen() const {
  return !glfwWindowShouldClose(glfw_window_);
}

void GlfwWindow::Center(bool center_horizontal, bool center_vertical) {
  GLFW::CenterWindow(glfw_window_, center_horizontal, center_vertical);
}

void GlfwWindow::PollEvents() {
  if (num_glfw_windows_ > 0) {
    glfwPollEvents();
  }
}

}
