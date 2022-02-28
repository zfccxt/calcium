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

  glfwSetWindowUserPointer(glfw_window_, this);
  glfwSetKeyCallback(glfw_window_, PerformKeyCallbacks);
  glfwSetFramebufferSizeCallback(glfw_window_, PerformFramebufferSizeCallbacks);
  glfwSetMouseButtonCallback(glfw_window_, PerformMouseButtonCallbacks);
  glfwSetScrollCallback(glfw_window_, PerformMouseWheelCallbacks);
  glfwSetCursorPosCallback(glfw_window_, PerformMouseMoveCallbacks);
}

float GlfwWindow::GetAspectRatio() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  return (float)width / (float)height;
}

bool GlfwWindow::IsOpen() const {
  return !glfwWindowShouldClose(glfw_window_);
}

void GlfwWindow::Close() {
  glfwSetWindowShouldClose(glfw_window_, true);
}

void GlfwWindow::Center(bool center_horizontal, bool center_vertical) {
  GLFW::CenterWindow(glfw_window_, center_horizontal, center_vertical);
}

void GlfwWindow::PollEvents() {
  glfwPollEvents();
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

float GlfwWindow::CursorX() {
  double x, y;
  glfwGetCursorPos(glfw_window_, &x, &y);
  return x;
}

float GlfwWindow::CursorY() {
  double x, y;
  glfwGetCursorPos(glfw_window_, &x, &y);
  return y;
}

bool GlfwWindow::IsKeyDown(KeyCode key) {
  return glfwGetKey(glfw_window_, (int)key);
}

void GlfwWindow::SetKeyPressCallback(KeyCode key, KeyCallback callback) {
  key_press_callbacks_.emplace(key, callback);
}

void GlfwWindow::RemoveKeyPressCallback(KeyCode key) {
  key_press_callbacks_.erase(key);
}

void GlfwWindow::SetKeyReleaseCallback(KeyCode key, KeyCallback callback) {
  key_release_callbacks_.emplace(key, callback);
}

void GlfwWindow::RemoveKeyReleaseCallback(KeyCode key) {
  key_release_callbacks_.erase(key);
}

void GlfwWindow::PerformKeyCallbacks(GLFWwindow* glfw_window, int key, int scancode, int action, int mods) {
  GlfwWindow* win = (GlfwWindow*)glfwGetWindowUserPointer(glfw_window);
  if (action == GLFW_PRESS) {
    auto it = win->key_press_callbacks_.find((KeyCode)key);
    if (it != win->key_press_callbacks_.end()) {
      it->second();
    }
  }
  else {
    auto it = win->key_release_callbacks_.find((KeyCode)key);
    if (it != win->key_release_callbacks_.end()) {
      it->second();
    }
  }
}

void GlfwWindow::SetResizeCallback(ResizeCallback callback) {
  resize_callback_ = callback;
}

void GlfwWindow::RemoveResizeCallback() {
  resize_callback_ = nullptr;
}

void GlfwWindow::PerformFramebufferSizeCallbacks(GLFWwindow* glfw_window, int width, int height) {
  GlfwWindow* win = (GlfwWindow*)glfwGetWindowUserPointer(glfw_window);
  win->OnFramebufferResize(width, height);
  if (win->resize_callback_) {
    win->resize_callback_();
  }
}

void GlfwWindow::SetMouseButtonPressCallback(MouseButton button, MouseButtonCallback callback) {
  mouse_press_callbacks_.emplace(button, callback);
}

void GlfwWindow::RemoveMouseButtonPressCallback(MouseButton button) {
  mouse_press_callbacks_.erase(button);
}

void GlfwWindow::SetMouseButtonReleaseCallback(MouseButton button, MouseButtonCallback callback) {
  mouse_release_callbacks_.emplace(button, callback);
}

void GlfwWindow::RemoveMouseButtonReleaseCallback(MouseButton button) {
  mouse_release_callbacks_.erase(button);
}

void GlfwWindow::PerformMouseButtonCallbacks(GLFWwindow* glfw_window, int button, int action, int mods) {
  GlfwWindow* win = (GlfwWindow*)glfwGetWindowUserPointer(glfw_window);
  if (action == GLFW_PRESS) {
    auto it = win->mouse_press_callbacks_.find((MouseButton)button);
    if (it != win->mouse_press_callbacks_.end()) {
      it->second();
    }
  }
  else {
    auto it = win->mouse_release_callbacks_.find((MouseButton)button);
    if (it != win->mouse_release_callbacks_.end()) {
      it->second();
    }
  }
}

void GlfwWindow::SetMouseWheelCallback(MouseWheelCallback callback) {
  mouse_wheel_callback_ = callback;
}

void GlfwWindow::RemoveMouseWheelCallback() {
  mouse_wheel_callback_ = nullptr;
}

void GlfwWindow::PerformMouseWheelCallbacks(GLFWwindow* glfw_window, double xoffs, double yoffs) {
  GlfwWindow* win = (GlfwWindow*)glfwGetWindowUserPointer(glfw_window);
  if (win->mouse_wheel_callback_) {
    win->mouse_wheel_callback_(xoffs, yoffs);
  }
}

void GlfwWindow::PerformMouseMoveCallbacks(GLFWwindow* glfw_window, double xoffs, double yoffs) {
  GlfwWindow* win = (GlfwWindow*)glfwGetWindowUserPointer(glfw_window);
  if (win->mouse_move_callback_) {
    win->mouse_move_callback_(xoffs - win->cursor_last_x_, yoffs - win->cursor_last_y_);
  }
  win->cursor_last_x_ = xoffs;
  win->cursor_last_y_ = yoffs;
}

void GlfwWindow::SetMouseMoveCallback(MouseMoveCallback callback) {
  mouse_move_callback_ = callback;
}

void GlfwWindow::RemoveMouseMoveCallback() {
  mouse_move_callback_ = nullptr;
}


}
