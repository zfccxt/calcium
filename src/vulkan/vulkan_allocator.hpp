#pragma once

#include <vulkan/vulkan.h>

namespace cl::Vulkan {

VkAllocationCallbacks* CreateAllocator();
void DestroyAllocator(VkAllocationCallbacks* allocator);

}
