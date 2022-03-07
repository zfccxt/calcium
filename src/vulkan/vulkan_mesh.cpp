#include "vulkan_mesh.hpp"

#include "instrumentor.hpp"
#include "vulkan_buffer_utils.hpp"
#include "vulkan_check.hpp"
#include "vulkan_window.hpp"

namespace cl::vulkan {

VulkanMesh::VulkanMesh(VulkanContextData* context, const MeshCreateInfo& mesh_info) : context_(context) {
  CALCIUM_PROFILE_FUNCTION();

  num_indices_ = mesh_info.indices.size();

  CreateVertexBuffer(mesh_info);
  CreateIndexBuffer(mesh_info);
}

void VulkanMesh::CreateVertexBuffer(const MeshCreateInfo& create_info) {
  CALCIUM_PROFILE_FUNCTION();

  VkDeviceSize buffer_size = sizeof(create_info.vertices[0]) * create_info.vertices.size();

  // GPU memory that is not visible or accessible by the CPU tends to be a lot faster, so we must use a staging buffer
  // to transfer vertex data to the GPU
  // This means we will copy the data first to a buffer that is accessible by the CPU, then copy it from there to a
  // fast, GPU-only buffer
  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_memory;
  CreateBuffer(context_, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
    staging_buffer, staging_buffer_memory);

  // We map the allocated memory to be accessible by the cpu and simply use memcpy to copy the vertex data into it
  void* data;
  VK_CHECK(vkMapMemory(context_->device, staging_buffer_memory, 0, buffer_size, 0, &data));
  memcpy(data, create_info.vertices.data(), (size_t)buffer_size);
  vkUnmapMemory(context_->device, staging_buffer_memory);

  // Create and allocate memory for the vertex buffer - this time we want a device local memory buffer
  CreateBuffer(context_, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_buffer_, vertex_buffer_memory_);

  // Copy the data from the staging buffer to the vertex buffer
  CopyBuffer(context_, staging_buffer, vertex_buffer_, buffer_size);

  // Cleanup the staging buffer
  vkDestroyBuffer(context_->device, staging_buffer, context_->allocator);
  vkFreeMemory(context_->device, staging_buffer_memory, context_->allocator);
}

void VulkanMesh::CreateIndexBuffer(const MeshCreateInfo& create_info) {
  CALCIUM_PROFILE_FUNCTION();

  // Almost a direct copy of CreateVertexBuffer except for a few differences
  // The buffer size is different
  VkDeviceSize buffer_size = sizeof(create_info.indices[0]) * create_info.indices.size();

  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_memory;
  CreateBuffer(context_, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    staging_buffer, staging_buffer_memory);
  
  void* data;
  VK_CHECK(vkMapMemory(context_->device, staging_buffer_memory, 0, buffer_size, 0, &data));
  memcpy(data, create_info.indices.data(), (size_t)buffer_size);
  vkUnmapMemory(context_->device, staging_buffer_memory);
  
  // The buffer usage is now VK_BUFFER_USAGE_INDEX_BUFFER_BIT
  CreateBuffer(context_, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index_buffer_, index_buffer_memory_);
  
  CopyBuffer(context_, staging_buffer, index_buffer_, buffer_size);
  
  vkDestroyBuffer(context_->device, staging_buffer, context_->allocator);
  vkFreeMemory(context_->device, staging_buffer_memory, context_->allocator);
}

VulkanMesh::~VulkanMesh() {
  vkDeviceWaitIdle(context_->device);

  vkDestroyBuffer(context_->device, vertex_buffer_, context_->allocator);
  vkFreeMemory(context_->device, vertex_buffer_memory_, context_->allocator);
  vkDestroyBuffer(context_->device, index_buffer_, context_->allocator);
  vkFreeMemory(context_->device, index_buffer_memory_, context_->allocator);
}

void VulkanMesh::Draw() {
  CALCIUM_PROFILE_FUNCTION();

  constexpr VkDeviceSize offsets[] = { 0 };
  VkBuffer vertex_buffers[] = { vertex_buffer_ };
  auto render_target = context_->bound_render_target.lock();
  VkCommandBuffer render_command_buffer = render_target->GetCurrentRenderCommandBuffer();
  vkCmdBindVertexBuffers(render_command_buffer, 0, 1, vertex_buffers, offsets);
  vkCmdBindIndexBuffer(render_command_buffer, index_buffer_, 0, VK_INDEX_TYPE_UINT32);
  vkCmdDrawIndexed(render_command_buffer, num_indices_, 1, 0, 0, 0);
}

}
