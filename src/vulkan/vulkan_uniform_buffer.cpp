#include "vulkan_uniform_buffer.hpp"

#include "vulkan_buffer_utils.hpp"
#include "vulkan_constants.hpp"
#include "vulkan_window.hpp"

namespace cl::Vulkan {

VulkanUniformBuffer::VulkanUniformBuffer(VulkanContextData* context, const ShaderReflectionDetails::UniformData& descriptor) : context_(context) {
  size_ = descriptor.size;

  buffers_.resize(kMaxFramesInFlight);
  buffers_memory_.resize(kMaxFramesInFlight);

  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    CreateBuffer(context, size_, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffers_[i], buffers_memory_[i]);
  }
}

VulkanUniformBuffer::~VulkanUniformBuffer() {
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    vkDestroyBuffer(context_->device, buffers_[i], context_->allocator);
    vkFreeMemory(context_->device, buffers_memory_[i], context_->allocator);
  }
}

void VulkanUniformBuffer::UploadData(void* to_upload) {
  void* data;
  auto bound_render_target = context_->bound_render_target.lock();
  size_t index = bound_render_target->GetCurrentFrameIndex();
  vkMapMemory(context_->device, buffers_memory_[index], 0, size_, 0, &data);
  memcpy(data, to_upload, size_);
  vkUnmapMemory(context_->device, buffers_memory_[index]);
}

}
