#include <calcium.hpp>

int main() {
  auto context = cl::CreateContext(cl::Backend::kOpenGL);

  cl::WindowCreateInfo window_info;
  window_info.clear_colour = 0x336699ff;
  window_info.enable_depth_test = false;
  auto window = context->CreateWindow(window_info);

  cl::ShaderCreateInfo shader_info;
  shader_info.vert_path = "res/shaders/shader.vert.spv";
  shader_info.frag_path = "res/shaders/shader.frag.spv";
  auto shader = context->CreateShader(shader_info);


  while (window->IsOpen()) {
    cl::PollWindowEvents();

    context->BindRendertarget(window);
    window->Clear();
    window->SwapBuffers();
  }
}
