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

  window->SetKeyPressCallback(cl::KeyCode::kEscape, [&](){ window->Close(); });

  glm::mat4 projection = glm::mat4(1);
  cl::ResizeCallback calc_projection = [&](){ projection = glm::perspective(glm::radians(45.0f), window->GetAspectRatio(), 0.1f, 1000.0f); };
  window->SetResizeCallback(calc_projection);
  calc_projection();

  auto shader = context->CreateShader("res/shaders/shader.vert.spv",  "res/shaders/shader.frag.spv");
  
  auto mesh = context->CreateMesh("res/models/drill.obj");
  
  auto texture = context->CreateTexture("res/models/drill_diffuse.png");
  
  auto start_time = std::chrono::high_resolution_clock::now();
  while (window->IsOpen()) {
    window->PollEvents();

    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    glm::mat4 viewprojection = projection * glm::translate(glm::mat4(1), glm::vec3(0, sin(time), -3));
    shader->UploadUniform("u_viewprojection", glm::value_ptr(viewprojection));

    glm::mat4 model = glm::rotate(glm::mat4(1), time, glm::vec3(0, 0, 1));
    shader->UploadUniform("u_model", glm::value_ptr(model));
    
    shader->BindTexture("u_diffuse_texture", texture);

    context->BeginRenderPass(shader);
    mesh->Draw();
    context->EndRenderPass();
  }

}
