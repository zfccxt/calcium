#pragma once

#include <assert.h>

#ifdef CALCIUM_BUILD_DEBUG
  #include <vulkan/vulkan.h>

namespace cl::vulkan {
  void PrintVulkanError(VkResult result);
}

  #define VK_CHECK(x) if ((x != VK_SUCCESS)) { ::cl::vulkan::PrintVulkanError(x); assert(false); }
#else
  #define VK_CHECK(x) x
#endif
