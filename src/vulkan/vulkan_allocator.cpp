#include "vulkan_allocator.hpp"

namespace cl::vulkan {

VkAllocationCallbacks* CreateAllocator(const VulkanContextData& context_data) {
  // Unused
  return nullptr;
}

void DestroyAllocator(const VulkanContextData& context_data, VkAllocationCallbacks* allocator) {
  // Unused
}

}
