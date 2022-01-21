#include "window.hpp"

#include "glfw_window.hpp"

namespace cl {

void PollWindowEvents() {
  GlfwWindow::PollEvents();
}

}
