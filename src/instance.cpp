#include "instance.hpp"

#include "opengl/opengl_instance.hpp"
#include "vulkan/vulkan_instance.hpp"

namespace cl {

std::unique_ptr<Instance> CreateInstance(Backend backend) {
  if (backend == Backend::kOpenGL) {
    return std::make_unique<OpenGL::OpenGLInstance>();
  }
  else {
    return std::make_unique<Vulkan::VulkanInstance>();
  }
}

}