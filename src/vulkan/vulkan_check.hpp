#pragma once

#include <assert.h>

#ifdef CALCIUM_BUILD_DEBUG
  #include <vulkan/vulkan.h>

namespace cl::Vulkan {
  void PrintVulkanError(VkResult result);
}

  #define VK_CHECK(x) if ((x != VK_SUCCESS)) { ::cl::Vulkan::PrintVulkanError(x); assert(false); }
#else
  #define VK_CHECK(x) x
#endif
