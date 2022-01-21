#include "vulkan_context.hpp"

#include "debug_call.hpp"
#include "glfw_utils.hpp"
#include "vulkan/vulkan_allocator.hpp"
#include "vulkan/vulkan_debug_messenger.hpp"
#include "vulkan/vulkan_instance.hpp"
#include "vulkan/vulkan_window.hpp"

namespace cl::Vulkan {

VulkanContext::VulkanContext() {
  GLFW::IncrementGLFWContextCount();
  context_data_.allocator       = CreateAllocator(context_data_);
  context_data_.instance        = CreateInstance(context_data_);
#ifdef CALCIUM_BUILD_DEBUG
  context_data_.debug_messenger = CreateDebugMessenger(context_data_);
#endif
}

VulkanContext::~VulkanContext() {
#ifdef CALCIUM_BUILD_DEBUG
  DestroyDebugMessenger(context_data_, context_data_.debug_messenger);
#endif
  DestroyInstance(context_data_, context_data_.instance);
  DestroyAllocator(context_data_, context_data_.allocator);
  GLFW::DecrementGLFWContextCount();
}

std::unique_ptr<Window> VulkanContext::CreateWindow() {
  WindowCreateInfo create_info;
  return std::make_unique<VulkanWindow>(&context_data_, create_info);
}

std::unique_ptr<Window> VulkanContext::CreateWindow(size_t width, size_t height) {
  WindowCreateInfo create_info;
  create_info.width = width;
  create_info.height = height;
  return std::make_unique<VulkanWindow>(&context_data_, create_info);
}

std::unique_ptr<Window> VulkanContext::CreateWindow(const WindowCreateInfo& create_info) {
  return std::make_unique<VulkanWindow>(&context_data_, create_info);
}

}
