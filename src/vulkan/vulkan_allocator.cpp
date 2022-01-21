#include "vulkan_allocator.hpp"

namespace cl::Vulkan {

VkAllocationCallbacks* CreateAllocator(const VulkanContextData& context_data) {
  // Unused
  return nullptr;
}

void DestroyAllocator(VkAllocationCallbacks* allocator) {
  // Unused
}

}
