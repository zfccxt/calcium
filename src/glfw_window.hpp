#pragma once

#include <cstddef>

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

protected:
  void CreateGlfwWindow(const WindowCreateInfo& create_info);

protected:
  GLFWwindow* glfw_window_ = nullptr;

private:
  static size_t num_glfw_windows_;

  double cursor_x_,     cursor_y_;
  double cursor_last_x_, cursor_last_y_;
};

}
