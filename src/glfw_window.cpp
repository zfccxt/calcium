#include "glfw_window.hpp"

#include <assert.h>

#include <GLFW/glfw3.h>

namespace cl {

size_t GlfwWindow::num_glfw_windows_ = 0;

GlfwWindow::GlfwWindow() {
  if (num_glfw_windows_ == 0) {
    glfwInit();
  }
  ++num_glfw_windows_;
}

GlfwWindow::~GlfwWindow() {
  --num_glfw_windows_;
  if (num_glfw_windows_ == 0) {
    glfwTerminate();
  }
}

void GlfwWindow::CreateGlfwWindow(const WindowCreateInfo& create_info) {
  glfw_window_ = glfwCreateWindow(create_info.width, create_info.height, create_info.title.c_str(), nullptr, nullptr);
  assert(glfw_window_);
}

bool GlfwWindow::IsOpen() const {
  return !glfwWindowShouldClose(glfw_window_);
}

void GlfwWindow::PollEvents() const {
  glfwPollEvents();
}

}
