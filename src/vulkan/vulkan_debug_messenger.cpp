#include "vulkan_debug_messenger.hpp"

#include <assert.h>
#include <cstdio>

#include "instrumentor.hpp"

namespace cl::Vulkan {

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
  fprintf(stderr, "[Vulkan] [Validation]: %s\n", callback_data->pMessage);
  return VK_FALSE;
}

VkDebugUtilsMessengerEXT CreateDebugMessenger(const VulkanContextData& context_data) {
  VkDebugUtilsMessengerCreateInfoEXT create_info { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
  // Only output verbose messages in verbose builds
  create_info.messageSeverity = 
#ifdef CALCIUM_BUILD_VERBOSE
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
#endif
                                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                              | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                          | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                          | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = DebugCallback;
  create_info.pUserData = nullptr; // User data is optional - similar to GLFW window user pointer
  
  // We have to dynamically load a pointer to the function that creates a debug messenger
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(context_data.instance, "vkCreateDebugUtilsMessengerEXT");
  assert(func);

  VkDebugUtilsMessengerEXT debug_messenger;
  func(context_data.instance, &create_info, context_data.allocator, &debug_messenger);
  assert(debug_messenger);
  return debug_messenger;
}

void DestroyDebugMessenger(const VulkanContextData& context_data, VkDebugUtilsMessengerEXT debug_messenger) {
  // We have to load the function to destroy the debug messenger
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(context_data.instance, "vkDestroyDebugUtilsMessengerEXT");
  // No point asserting, since we are destroying the context anyway
  if (func) {
    func(context_data.instance, debug_messenger, context_data.allocator);
  }
}

}
