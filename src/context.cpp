#include "context.hpp"

#include "opengl/opengl_context.hpp"
#ifdef CALCIUM_VULKAN_SDK_FOUND
  #include "vulkan/vulkan_context.hpp"
#endif

#include "instrumentor.hpp"

namespace cl {

std::shared_ptr<Window> Context::CreateWindow() {
  WindowCreateInfo window_info;
  return CreateWindow(window_info);
}

std::shared_ptr<Window> Context::CreateWindow(size_t width, size_t height) {
  WindowCreateInfo window_info;
  window_info.width = width;
  window_info.height = height;
  return CreateWindow(window_info);
}

std::shared_ptr<Shader> Context::CreateShader(const std::string& vert_path, const std::string& frag_path) {
  ShaderCreateInfo shader_info;
  shader_info.vert_path = vert_path;
  shader_info.frag_path = frag_path;
  return CreateShader(shader_info);
}

std::shared_ptr<Mesh> Context::CreateMesh(const MeshData& mesh_data) {
  MeshCreateInfo mesh_info;
  mesh_info.vertex_input_layout = mesh_data.vertex_input_layout;
  mesh_info.vertices = mesh_data.vertices.data();
  mesh_info.num_vertices = mesh_data.vertices.size();
  mesh_info.indices = mesh_data.indices.data();
  mesh_info.num_indices = mesh_data.indices.size();
  return CreateMesh(mesh_info);
}

std::shared_ptr<Mesh> Context::CreateMesh(const std::string& file_path) {
  // TODO: Support more mesh file types
  // TODO: switch (file extension)
  // case .obj
  MeshData mesh_data = Mesh::LoadObj(file_path);
  return CreateMesh(mesh_data);
}

std::shared_ptr<Texture> Context::CreateTexture(const std::string& file_path) {
  TextureCreateInfo texture_info;
  texture_info.file_path = file_path;
  texture_info.filter = TextureFilter::kLinear;
  texture_info.wrap = TextureWrap::kRepeat;
  return CreateTexture(texture_info);
}

std::shared_ptr<Context> CreateContext(Backend backend) {
  CALCIUM_PROFILE_BEGIN("profile_results.json");

#ifdef CALCIUM_VULKAN_SDK_FOUND
  if (backend == Backend::kVulkan) {
    return std::make_shared<Vulkan::VulkanContext>();
  }
#endif

  return std::make_shared<OpenGL::OpenGLContext>();
}

Context::~Context() {
  CALCIUM_PROFILE_END();
}

}
