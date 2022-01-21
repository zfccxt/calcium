#include "vulkan_context.hpp"

#include "vulkan/vulkan_allocator.hpp"
#include "vulkan/vulkan_instance.hpp"
#include "vulkan/vulkan_window.hpp"

namespace cl::Vulkan {

VulkanContext::VulkanContext() {
  context_data_.allocator = CreateAllocator(context_data_);
  context_data_.instance  = CreateInstance(context_data_);
}

VulkanContext::~VulkanContext() {
  DestroyInstance(context_data_, context_data_.instance);
  DestroyAllocator(context_data_, context_data_.allocator);
}

std::unique_ptr<Window> VulkanContext::CreateWindow() const {
  WindowCreateInfo create_info;
  return std::make_unique<VulkanWindow>(create_info);
}

std::unique_ptr<Window> VulkanContext::CreateWindow(size_t width, size_t height) const {
  WindowCreateInfo create_info;
  create_info.width = width;
  create_info.height = height;
  return std::make_unique<VulkanWindow>(create_info);
}

std::unique_ptr<Window> VulkanContext::CreateWindow(const WindowCreateInfo& create_info) const {
  return std::make_unique<VulkanWindow>(create_info);
}

}
