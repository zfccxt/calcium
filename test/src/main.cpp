#include <calcium.hpp>

int main() {
  auto context = cl::CreateContext(cl::Backend::kVulkan);

  auto window = context->CreateWindow();

  while (window->IsOpen()) {
    cl::PollWindowEvents();
  }
}
