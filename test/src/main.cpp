#include <calcium.hpp>

int main() {
  auto context = cl::CreateContext(cl::Backend::kOpenGL);

  auto window = context->CreateWindow();

  cl::ShaderCreateInfo shader_info;
  auto shader = context->CreateShader(shader_info);

  while (window->IsOpen()) {
    cl::PollWindowEvents();
  }
}
