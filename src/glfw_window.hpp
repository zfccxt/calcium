#pragma once

#include <cstddef>
#include <unordered_map>

#include "callbacks.hpp"
#include "window.hpp"
#include "window_create_info.hpp"

struct GLFWwindow;

namespace cl {

class GlfwWindow : public Window {
public:
  GlfwWindow();
  virtual ~GlfwWindow();

  virtual float GetAspectRatio() const override;
  virtual bool IsOpen() const override;
  virtual void Close() override;
  virtual void Center(bool center_horizontal = true, bool center_vertical = true) override;

  virtual void PollEvents() override;

  virtual bool IsCursorLocked() override;

  virtual void LockCursor() override;
  virtual void UnlockCursor() override;
  virtual void ToggleCursorLock() override;

  virtual float CursorX() override;
  virtual float CursorY() override;

  virtual bool IsKeyDown(KeyCode key) override;

  virtual void SetKeyPressCallback(KeyCode, KeyCallback) override;
  virtual void RemoveKeyPressCallback(KeyCode) override;
  virtual void SetKeyReleaseCallback(KeyCode, KeyCallback) override;
  virtual void RemoveKeyReleaseCallback(KeyCode) override;

  virtual void SetResizeCallback(ResizeCallback) override;
  virtual void RemoveResizeCallback() override;

  virtual void SetMouseButtonPressCallback(MouseButton, MouseButtonCallback) override;
  virtual void RemoveMouseButtonPressCallback(MouseButton) override;
  virtual void SetMouseButtonReleaseCallback(MouseButton, MouseButtonCallback) override;
  virtual void RemoveMouseButtonReleaseCallback(MouseButton) override;
  virtual void SetMouseWheelCallback(MouseWheelCallback) override;
  virtual void RemoveMouseWheelCallback() override;
  virtual void SetMouseMoveCallback(MouseMoveCallback) override;
  virtual void RemoveMouseMoveCallback() override;

  virtual bool IsControllerDetected() override;
  virtual bool IsControllerButtonDown(ControllerButton) override;

  virtual float ControllerLeftStickX() override;
  virtual float ControllerLeftStickY() override;
  virtual float ControllerRightStickX() override;
  virtual float ControllerRightStickY() override;

private:
  float GetControllerJoystickAxis(int axis);

public:
  virtual void SetControllerDeadzone(float size) override;

public:
  inline GLFWwindow* GetGlfwWindow() const { return glfw_window_; }

protected:
  void CreateGlfwWindow(const WindowCreateInfo& create_info);

private:
  static void PerformKeyCallbacks(GLFWwindow* glfw_window, int key, int scancode, int action, int mods);
  static void PerformFramebufferSizeCallbacks(GLFWwindow* glfw_window, int width, int height);
  static void PerformMouseButtonCallbacks(GLFWwindow* glfw_window, int button, int action, int mods);
  static void PerformMouseWheelCallbacks(GLFWwindow* glfw_window, double xoffset, double yoffset);
  static void PerformMouseMoveCallbacks(GLFWwindow* glfw_window, double xoffset, double yoffset);

protected:
  virtual void OnFramebufferResize(int width, int height) = 0;

protected:
  GLFWwindow* glfw_window_ = nullptr;

private:
  static size_t num_glfw_windows_;

  double cursor_last_x_ = 0.0, cursor_last_y_ = 0.0;

  std::unordered_map<KeyCode, KeyCallback> key_press_callbacks_;
  std::unordered_map<KeyCode, KeyCallback> key_release_callbacks_;
  std::unordered_map<MouseButton, MouseButtonCallback> mouse_press_callbacks_;
  std::unordered_map<MouseButton, MouseButtonCallback> mouse_release_callbacks_;
  MouseWheelCallback mouse_wheel_callback_ = nullptr;
  ResizeCallback resize_callback_ = nullptr;
  MouseMoveCallback mouse_move_callback_ = nullptr;

  float controller_deadzone_ = 0.0f;
};

}
