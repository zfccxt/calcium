#include "vulkan_uniform_buffer.hpp"

#include "instrumentor.hpp"
#include "vulkan_buffer_utils.hpp"
#include "vulkan_constants.hpp"
#include "vulkan_window.hpp"

namespace cl::vulkan {

VulkanUniformBuffer::VulkanUniformBuffer(VulkanContextData* context, const ShaderReflectionDetails::UniformData& descriptor) : context_(context) {
  CALCIUM_PROFILE_FUNCTION();

  size = descriptor.size;

  buffers.resize(kMaxFramesInFlight);
  buffers_memory.resize(kMaxFramesInFlight);

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    CreateBuffer(context, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffers[i], buffers_memory[i]);
  }
}

VulkanUniformBuffer::~VulkanUniformBuffer() {
  CALCIUM_PROFILE_FUNCTION();

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    vkDestroyBuffer(context_->device, buffers[i], context_->allocator);
    vkFreeMemory(context_->device, buffers_memory[i], context_->allocator);
  }
}

void VulkanUniformBuffer::UploadData(void* to_upload) {
  CALCIUM_PROFILE_FUNCTION();

  void* data;
  auto bound_render_target = context_->bound_render_target.lock();
  size_t index = bound_render_target->GetNextFrameIndex();
  vkMapMemory(context_->device, buffers_memory[index], 0, size, 0, &data);
  memcpy(data, to_upload, size);
  vkUnmapMemory(context_->device, buffers_memory[index]);
}

}
