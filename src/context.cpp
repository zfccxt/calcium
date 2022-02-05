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

std::unique_ptr<Context> CreateContext(Backend backend) {
#ifdef CALCIUM_VULKAN_SDK_FOUND
  if (backend == Backend::kVulkan) {
    return std::make_unique<Vulkan::VulkanContext>();
  }
#endif

  return std::make_unique<OpenGL::OpenGLContext>();
}

}
