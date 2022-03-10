#include <chrono>

#include <calcium.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
  auto context = cl::Context::CreateContext(cl::Backend::kVulkan);

  cl::WindowCreateInfo window_info;
  window_info.clear_colour = 0x336699ff;
  window_info.front_face = cl::WindingOrder::kCounterClockwise;
  auto window = context->CreateWindow(window_info);

  window->SetKeyPressCallback(cl::KeyCode::kEscape, [&](){ window->Close(); });

  glm::mat4 projection = glm::mat4(1);
  cl::ResizeCallback calc_projection = [&](){ projection = glm::perspective(glm::radians(45.0f), window->GetAspectRatio(), 0.1f, 1000.0f); };
  window->SetResizeCallback(calc_projection);
  calc_projection();

  auto shader = context->CreateShader("res/shaders/shader.vert.spv", "res/shaders/shader.frag.spv");
  
  auto mesh = context->CreateMesh("res/models/drill.obj");
  
  cl::TextureCreateInfo texture_info;
  texture_info.file_path = "res/models/drill_diffuse.png";
  texture_info.flip_vertical_on_load = true;
  auto texture = context->CreateTexture(texture_info);
  glm::mat4 invert = glm::scale(glm::mat4(1), glm::vec3(1, -1, 1));
  
  auto start_time = std::chrono::high_resolution_clock::now();
  while (window->IsOpen()) {
    window->PollEvents();

    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    glm::mat4 viewprojection = projection * glm::translate(glm::mat4(1), glm::vec3(0, 0.1f, -0.4f));
    shader->UploadUniform("u_viewprojection", glm::value_ptr(viewprojection));

    glm::mat4 model = invert * glm::rotate(glm::mat4(1), time, glm::vec3(0, 1, 0));
    shader->UploadUniform("u_model", glm::value_ptr(model));
    
    shader->BindTexture("u_diffuse_texture", texture);

    context->BeginFrame();
      shader->Bind();
      mesh->Draw();
    context->EndFrame();
  }

}
