#include "vulkan_instance.hpp"

#include "vulkan/vulkan_window.hpp"

namespace cl::Vulkan {

std::unique_ptr<Window> VulkanInstance::CreateWindow(const WindowCreateInfo& create_info) const {
  return std::make_unique<VulkanWindow>(create_info);
}

}
