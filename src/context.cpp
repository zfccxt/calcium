#include "context.hpp"

#include "opengl/opengl_context.hpp"
#ifdef CALCIUM_VULKAN_SDK_FOUND
  #include "vulkan/vulkan_context.hpp"
#endif

namespace cl {

std::unique_ptr<Context> CreateContext(Backend backend) {
#ifdef CALCIUM_VULKAN_SDK_FOUND
  if (backend == Backend::kVulkan) {
    return std::make_unique<Vulkan::VulkanContext>();
  }
#endif

  return std::make_unique<OpenGL::OpenGLContext>();
}

}
