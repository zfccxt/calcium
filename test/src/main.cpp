#include <calcium.hpp>

int main() {
  auto instance = cl::CreateInstance(cl::Backend::kOpenGL);

  cl::WindowCreateInfo window_info;
  auto window = instance->CreateWindow(window_info);

  while (window->IsOpen()) {
    cl::PollWindowEvents();
  }
}
