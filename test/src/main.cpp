#include <chrono>

#include <calcium.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
  auto context = cl::CreateContext(cl::Backend::kVulkan);

  cl::WindowCreateInfo window_info;
  window_info.clear_colour = 0x336699ff;
  window_info.enable_depth_test = false;
  auto window = context->CreateWindow(window_info);

  window->SetKeyPressCallback(cl::KeyCode::kEscape, [&](){ window->Close(); });

  glm::mat4 projection = glm::mat4(1);
  cl::ResizeCallback calc_projection = [&](){ projection = glm::perspective(glm::radians(45.0f), window->GetAspectRatio(), 0.1f, 1000.0f); };
  window->SetResizeCallback(calc_projection);
  calc_projection();

  auto shader          = context->CreateShader("res/shaders/shader.vert.spv",  "res/shaders/shader.frag.spv");
  auto no_input_shader = context->CreateShader("res/shaders/noinput.vert.spv", "res/shaders/noinput.frag.spv");

  cl::MeshCreateInfo mesh_info;
  mesh_info.vertex_data_layout = shader->GetInputLayout();
  std::vector<float> vertices = {
  //  x      y     z     u     v
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
  };
  std::vector<uint32_t> indices = {
    0, 1, 2,
    1, 3, 2,
  };
  mesh_info.vertices = vertices.data();
  mesh_info.num_vertices = vertices.size();
  mesh_info.indices = indices.data();
  mesh_info.num_indices = indices.size();
  auto mesh = context->CreateMesh(mesh_info);

  auto texture = context->CreateTexture("res/textures/pepper.png");
  auto texture2 = context->CreateTexture("res/textures/face.png");

  auto start_time = std::chrono::high_resolution_clock::now();
  while (window->IsOpen()) {
    window->PollEvents();

    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    context->BeginRenderPass(shader);

    glm::mat4 viewprojection = projection * glm::translate(glm::mat4(1), glm::vec3(0, sin(time), -3));
    shader->UploadUniform("u_viewprojection", glm::value_ptr(viewprojection));

    glm::mat4 model = glm::rotate(glm::mat4(1), time, glm::vec3(0, 0, 1));
    shader->UploadUniform("u_model", glm::value_ptr(model));
    no_input_shader->UploadUniform("u_model", glm::value_ptr(model));

    shader->BindTexture("u_diffuse_texture", texture);
    shader->BindTexture("u_second_texture", texture2);

    mesh->Draw();
    context->EndRenderPass();
  }

}
