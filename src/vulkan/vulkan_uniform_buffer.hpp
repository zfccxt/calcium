#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>

#include "shader_reflection_details.hpp"
#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

struct VulkanUniformBuffer {
public:
  VulkanUniformBuffer(VulkanContextData* context, const ShaderReflectionDetails::UniformData& descriptor);
  ~VulkanUniformBuffer();

  void UploadData(void* data);

public:
  // We have one memory buffer for each possible frame in flight since the uniform might be updated while rendering a frame
  std::vector<VkBuffer> buffers;
  std::vector<VkDeviceMemory> buffers_memory;
  VkDeviceSize size = 0;

private:
  VulkanContextData* context_;
};

typedef std::unordered_map<size_t, std::unique_ptr<VulkanUniformBuffer>> VulkanUniformMap;

}
