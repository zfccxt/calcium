#include <calcium.hpp>

int main() {
  auto context = cl::CreateContext(cl::Backend::kOpenGL);

  auto window = context->CreateWindow();

  cl::ShaderCreateInfo shader_info;
  shader_info.vert_path = "res/shaders/shader.vert.glsl";
  shader_info.vert_path = "res/shaders/shader.frag.glsl";
  auto shader = context->CreateShader(shader_info);

  while (window->IsOpen()) {
    cl::PollWindowEvents();

    context->BindRendertarget(window);
    window->SwapBuffers();
  }
}
