#include "vulkan_context.hpp"

#include <GLFW/glfw3.h>

#include "debug_call.hpp"
#include "glfw_utils.hpp"
#include "vulkan/vulkan_allocator.hpp"
#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_command_pool.hpp"
#include "vulkan/vulkan_debug_messenger.hpp"
#include "vulkan/vulkan_device.hpp"
#include "vulkan/vulkan_instance.hpp"
#include "vulkan/vulkan_physical_device.hpp"
#include "vulkan/vulkan_window.hpp"

namespace cl::Vulkan {

VulkanContext::VulkanContext() {
  GLFW::IncrementGLFWContextCount();
  context_data_.allocator       = CreateAllocator(context_data_);
  context_data_.instance        = CreateInstance(context_data_);
#ifdef CALCIUM_BUILD_DEBUG
  context_data_.debug_messenger = CreateDebugMessenger(context_data_);
#endif

  // We have to have a window surface in order to figure out which device to pick and how to create the command buffers
  // The device must be able to render to the surface and it must support the surface formats
  // The command pool needs to know which queues it will use, which we need the surface to find out
  // We create a temporary invisible window and surface and use these to figure out the capabilities of candidate GPUs
  // This is a bit silly and there is likely a much better solution
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow* temp_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
  VkSurfaceKHR temp_surface;
  VK_CHECK(glfwCreateWindowSurface(context_data_.instance, temp_window, context_data_.allocator, &temp_surface));

  context_data_.physical_device = ChoosePhysicalDevice(context_data_, temp_surface);
  context_data_.device = CreateDevice(context_data_, temp_surface);
  context_data_.graphics_queue = FindGraphicsQueue(context_data_, temp_surface);
  context_data_.present_queue = FindPresentQueue(context_data_, temp_surface);
  context_data_.command_pool = CreateCommandPool(context_data_, temp_surface);

  vkDestroySurfaceKHR(context_data_.instance, temp_surface, context_data_.allocator);
  glfwDestroyWindow(temp_window);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
}

VulkanContext::~VulkanContext() {
  DestroyCommandPool(context_data_, context_data_.command_pool);
  DestroyDevice(context_data_, context_data_.device);

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
