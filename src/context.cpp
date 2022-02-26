#include "context.hpp"

#include "opengl/opengl_context.hpp"
#ifdef CALCIUM_VULKAN_SDK_FOUND
  #include "vulkan/vulkan_context.hpp"
#endif

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

std::shared_ptr<Mesh> Context::CreateMesh(const std::string& file_path) {
  // TODO: Support more mesh file types
  // TODO: switch (file extension)
  // case .obj
  MeshCreateInfo create_info = Mesh::LoadObjFile(file_path);
  return CreateMesh(create_info);
}

std::shared_ptr<Texture> Context::CreateTexture(const std::string& file_path) {
  TextureCreateInfo texture_info;
  texture_info.file_path = file_path;
  texture_info.filter = TextureFilter::kLinear;
  texture_info.wrap = TextureWrap::kRepeat;
  return CreateTexture(texture_info);
}

std::shared_ptr<Context> CreateContext(Backend backend) {
#ifdef CALCIUM_VULKAN_SDK_FOUND
  if (backend == Backend::kVulkan) {
    return std::make_shared<Vulkan::VulkanContext>();
  }
#endif

  return std::make_shared<OpenGL::OpenGLContext>();
}

}
