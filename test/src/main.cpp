#include <calcium.hpp>

int main() {
  auto instance = cl::CreateInstance(cl::Backend::kOpenGL);

  auto window = instance->CreateWindow();

  while (window->IsOpen()) {
    cl::PollWindowEvents();
  }
}
