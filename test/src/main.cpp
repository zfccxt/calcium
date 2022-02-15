#include <chrono>

#include <calcium.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
  mesh_info.vertex_data_layout = shader->GetInputLayout();
  std::vector<float> vertices = {
     0.5f,  0.5f, 0.0f,
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

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), window->GetAspectRatio(), 0.1f, 1000.0f);

  auto start_time = std::chrono::high_resolution_clock::now();
  context->BindRendertarget(window);
  while (window->IsOpen()) {
    cl::PollWindowEvents();

    window->Clear();

    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    glm::mat4 viewprojection = projection * glm::translate(glm::mat4(1), glm::vec3(0, sin(time), -3));
    shader->Bind();
    shader->UploadUniform("u_viewprojection", glm::value_ptr(viewprojection));

    glm::mat4 model = glm::rotate(glm::mat4(1), time, glm::vec3(0, 0, 1));
    shader->UploadUniform("u_model", glm::value_ptr(model));

    mesh->Draw();
    window->SwapBuffers();
  }
}
