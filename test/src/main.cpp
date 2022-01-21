#include <calcium.hpp>

int main() {
  auto context = cl::CreateContext(cl::Backend::kOpenGL);

  auto window = context->CreateWindow();

  while (window->IsOpen()) {
    cl::PollWindowEvents();
  }
}
