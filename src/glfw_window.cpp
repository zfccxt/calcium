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
  glfwWindowHint(GLFW_RESIZABLE, create_info.enable_resize);

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

float GlfwWindow::GetAspectRatio() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  return (float)width / (float)height;
}

bool GlfwWindow::IsOpen() const {
  return !glfwWindowShouldClose(glfw_window_);
}

void GlfwWindow::Center(bool center_horizontal, bool center_vertical) {
  GLFW::CenterWindow(glfw_window_, center_horizontal, center_vertical);
}

void GlfwWindow::PollEvents() {
  glfwPollEvents();

  cursor_last_x_ = cursor_x_;
  cursor_last_y_ = cursor_y_;
  glfwGetCursorPos(glfw_window_, &cursor_x_, &cursor_y_);
}

bool GlfwWindow::IsCursorLocked() {
  return glfwGetInputMode(glfw_window_, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

void GlfwWindow::LockCursor() {
  glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GlfwWindow::UnlockCursor() {
  glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GlfwWindow::ToggleCursorLock() {
  int mode = glfwGetInputMode(glfw_window_, GLFW_CURSOR);
  glfwSetInputMode(glfw_window_, GLFW_CURSOR, mode == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

float GlfwWindow::CursorDeltaX() {
  return (float)(cursor_y_ - cursor_last_y_);
}

float GlfwWindow::CursorDeltaY() {
  return (float)(cursor_x_ - cursor_last_x_);
}

float GlfwWindow::CursorX() {
  return cursor_x_;
}

float GlfwWindow::CursorY() {
  return cursor_y_;
}

bool GlfwWindow::IsKeyDown(KeyCode key) {
  return glfwGetKey(glfw_window_, (int)key);
}

}
