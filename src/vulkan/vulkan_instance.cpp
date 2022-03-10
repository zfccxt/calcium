#include "vulkan_instance.hpp"

#include <assert.h>

#include <GLFW/glfw3.h>

#include "debug_call.hpp"
#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"

namespace cl::vulkan {

const std::vector<const char*> kValidationLayers = { "VK_LAYER_KHRONOS_validation" };

void CheckValidationLayerSupport() {
  CALCIUM_PROFILE_FUNCTION();

  // Get list of all available instance layers
  // First call to vkEnumerateInstanceLayerProperties gets count of supported layers
  uint32_t layer_count = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  
  // Second call to vkEnumerateInstanceLayerProperties gets list of supported layers
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
  
  // Iterate over the list of requested validation layers, making sure each of them is
  // in the list of supported layers
  for (const char* layer_name : kValidationLayers) {
    bool layer_found = false;
    
    for (const auto& layer_props : available_layers) {
      if (strcmp(layer_name, layer_props.layerName) == 0) {
        layer_found = true;
        break;
      }
    }
    
    assert(layer_found);
  }
}

std::vector<const char*> GetRequiredInstanceExtensions() {
  CALCIUM_PROFILE_FUNCTION();

  // Get list of extensions required by GLFW
  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  
  std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
  
  // If in debug mode, we need an extension to create a debug messenger
  CALCIUM_DEBUG_CALL(extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME));

  // Required for VK_KHR_portability_subset which should be requested for MoltenVK
  // Also required for VK_EXT_extended_dynamic_state
  extensions.push_back("VK_KHR_get_physical_device_properties2");

  return extensions;
}

VkInstance CreateInstance(VulkanContextData& context_data) {
  CALCIUM_PROFILE_FUNCTION();

  CALCIUM_DEBUG_CALL(CheckValidationLayerSupport());

  // VkApplicationinfo is optional, but can help graphics drivers to optimise for this application/engine
  VkApplicationInfo app_info { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  // TODO: Request application name and version from client
  // app_info.pApplicationName = app_name;
  // app_info.applicationVersion = app_version;
  app_info.pEngineName = "Calcium";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;
  
  // We have to specify all required instance extensions up front at the time of instance creation
  auto instance_extensions = GetRequiredInstanceExtensions();
  
  // Create the instance
  VkInstanceCreateInfo instance_info { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  instance_info.pApplicationInfo = &app_info;
  instance_info.enabledExtensionCount = (uint32_t)instance_extensions.size();
  instance_info.ppEnabledExtensionNames = instance_extensions.data();
  
#if CALCIUM_BUILD_DEBUG
  instance_info.enabledLayerCount = (uint32_t)kValidationLayers.size();
  instance_info.ppEnabledLayerNames = kValidationLayers.data();
#else
  instance_info.enabledLayerCount = 0;
#endif
  
  VkInstance instance;
  VK_CHECK(vkCreateInstance(&instance_info, context_data.allocator, &instance));
  return instance;
}

void DestroyInstance(VulkanContextData& context_data, VkInstance instance) {
  vkDestroyInstance(instance, context_data.allocator);
}

}
