#include "instance.hpp"

#include "opengl/opengl_instance.hpp"
#ifdef CALCIUM_VULKAN_SDK_FOUND
  #include "vulkan/vulkan_instance.hpp"
#endif

namespace cl {

std::unique_ptr<Instance> CreateInstance(Backend backend) {
#ifdef CALCIUM_VULKAN_SDK_FOUND
  if (backend == Backend::kVulkan) {
    return std::make_unique<Vulkan::VulkanInstance>();
  }
#endif

  return std::make_unique<OpenGL::OpenGLInstance>();
}

}
