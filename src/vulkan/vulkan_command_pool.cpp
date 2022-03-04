#include "vulkan_command_pool.hpp"

#include "instrumentor.hpp"
#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_queue_family_indices.hpp"

namespace cl::vulkan {

VkCommandPool CreateCommandPool(const VulkanContextData& context_data, VkSurfaceKHR temp_surface) {
  CALCIUM_PROFILE_FUNCTION();

  VulkanQueueFamilyIndices indices(context_data.physical_device, temp_surface);
  VkCommandPoolCreateInfo create_info { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
  create_info.queueFamilyIndex = indices.graphics_family;
  create_info.flags = 0; // optional

  VkCommandPool command_pool;
  VK_CHECK(vkCreateCommandPool(context_data.device, &create_info, context_data.allocator, &command_pool));
  return command_pool;
}

void DestroyCommandPool(const VulkanContextData& context_data, VkCommandPool command_pool) {
  vkDestroyCommandPool(context_data.device, context_data.command_pool, context_data.allocator);
}

}
