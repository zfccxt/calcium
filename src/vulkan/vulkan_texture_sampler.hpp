#pragma once

#include <memory>
#include <unordered_map>

namespace cl::Vulkan {

struct VulkanTextureSampler {
  
};

typedef std::unordered_map<size_t, std::unique_ptr<VulkanTextureSampler>> VulkanTextureSamplerMap;

}
