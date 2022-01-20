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
  virtual void PollEvents() const override;

protected:
  void CreateGlfwWindow(const WindowCreateInfo& create_info);

private:
  GLFWwindow* glfw_window_ = nullptr;

  static size_t num_glfw_windows_;
};

}
