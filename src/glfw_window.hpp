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
  virtual void Center(bool center_horizontal = true, bool center_vertical = true) override;

  virtual void PollEvents() override;

  virtual bool IsCursorLocked() override;

  virtual void LockCursor() override;
  virtual void UnlockCursor() override;
  virtual void ToggleCursorLock() override;

  virtual float CursorDeltaX() override;
  virtual float CursorDeltaY() override;
  virtual float CursorX() override;
  virtual float CursorY() override;

  virtual bool IsKeyDown(KeyCode key) override;

  virtual void SetKeyPressCallback(KeyCode, KeyCallback) override;
  virtual void RemoveKeyPressCallback(KeyCode) override;
  virtual void SetKeyReleaseCallback(KeyCode, KeyCallback) override;
  virtual void RemoveKeyReleaseCallback(KeyCode) override;

  virtual void SetResizeCallback(ResizeCallback) override;
  virtual void RemoveResizeCallback() override;

protected:
  void CreateGlfwWindow(const WindowCreateInfo& create_info);

private:
  static void PerformKeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void PerformFramebufferSizeCallback(GLFWwindow* window, int width, int height);

protected:
  GLFWwindow* glfw_window_ = nullptr;

private:
  static size_t num_glfw_windows_;

  double cursor_x_      = 0.0, cursor_y_      = 0.0;
  double cursor_last_x_ = 0.0, cursor_last_y_ = 0.0;

  std::unordered_map<KeyCode, KeyCallback> key_press_callbacks_;
  std::unordered_map<KeyCode, KeyCallback> key_release_callbacks_;

  ResizeCallback resize_callback_ = nullptr;
};

}
