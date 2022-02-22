#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "shader_reflection_details.hpp"
#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanUniform {
public:
  VulkanUniform(VulkanContextData* context, const ShaderReflectionDetails::UniformData& descriptor);
  ~VulkanUniform();

  void UploadData(void* data);

private:
  VulkanContextData* context_;

  // We have one memory buffer for each possible frame in flight since the uniform might be updated while rendering a frame
  std::vector<VkBuffer> buffers_;
  std::vector<VkDeviceMemory> buffers_memory_;
  VkDeviceSize size_ = 0;
};

}
