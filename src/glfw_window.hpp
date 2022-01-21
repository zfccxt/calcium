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

  virtual bool IsOpen() const override;
  virtual void Center(bool center_horizontal = true, bool center_vertical = true) override;

  static void PollEvents();

protected:
  void CreateGlfwWindow(const WindowCreateInfo& create_info);

protected:
  GLFWwindow* glfw_window_ = nullptr;

private:
  static size_t num_glfw_windows_;
};

}
