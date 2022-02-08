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

  cl::MeshCreateInfo mesh_info;
  std::vector<float> vertices = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
  };
  std::vector<uint32_t> indices = {
    0, 1, 3,
    1, 2, 3,
  };
  mesh_info.vertices = vertices.data();
  mesh_info.num_vertices = vertices.size();
  mesh_info.indices = indices.data();
  mesh_info.num_indices = indices.size();
  auto mesh = context->CreateMesh(mesh_info);

  while (window->IsOpen()) {
    cl::PollWindowEvents();

    context->BindRendertarget(window);
    window->Clear();
    shader->Bind();
    window->SwapBuffers();
  }
}
