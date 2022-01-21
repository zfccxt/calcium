#include "vulkan_allocator.hpp"

namespace cl::Vulkan {

VkAllocationCallbacks* CreateAllocator() {
  // Unused
  return nullptr;
}

void DestroyAllocator(VkAllocationCallbacks* allocator) {
  // Unused
}

}
