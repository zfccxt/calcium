#include "vulkan_instance.hpp"

#include "vulkan/vulkan_window.hpp"

namespace cl::Vulkan {

std::unique_ptr<Window> VulkanInstance::CreateWindow() const {
  return std::unique_ptr<VulkanWindow>();
}

}
