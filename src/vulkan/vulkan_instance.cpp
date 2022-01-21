#include "vulkan_instance.hpp"

#include "vulkan/vulkan_window.hpp"

namespace cl::Vulkan {

std::unique_ptr<Window> VulkanInstance::CreateWindow() const {
  WindowCreateInfo create_info;
  return std::make_unique<VulkanWindow>(create_info);
}

std::unique_ptr<Window> VulkanInstance::CreateWindow(size_t width, size_t height) const {
  WindowCreateInfo create_info;
  create_info.width = width;
  create_info.height = height;
  return std::make_unique<VulkanWindow>(create_info);
}

std::unique_ptr<Window> VulkanInstance::CreateWindow(const WindowCreateInfo& create_info) const {
  return std::make_unique<VulkanWindow>(create_info);
}

}
