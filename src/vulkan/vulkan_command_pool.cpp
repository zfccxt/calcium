#include "vulkan_command_pool.hpp"

#include "vulkan/vulkan_check.hpp"
#include "vulkan/vulkan_queue_family_indices.hpp"

namespace cl::Vulkan {

VkCommandPool CreateCommandPool(const VulkanWindowData& window_data) {
  VulkanQueueFamilyIndices indices(window_data.physical_device, window_data.surface);
  VkCommandPoolCreateInfo create_info { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
  create_info.queueFamilyIndex = indices.graphics_family;
  create_info.flags = 0; // optional

  VkCommandPool command_pool;
  VK_CHECK(vkCreateCommandPool(window_data.device, &create_info, window_data.context_data->allocator, &command_pool));
  return command_pool;
}

void DestroyCommandPool(const VulkanWindowData& window_data, VkCommandPool command_pool) {
  vkDestroyCommandPool(window_data.device, window_data.command_pool, window_data.context_data->allocator);
}

}
