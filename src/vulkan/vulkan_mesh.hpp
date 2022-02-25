#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

#include "mesh.hpp"
#include "mesh_create_info.hpp"
#include "vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanMesh : public Mesh {
public:
  VulkanMesh(VulkanContextData* context, const MeshCreateInfo& mesh_info);
  ~VulkanMesh();

  virtual void Draw() override;

private:
  void CreateVertexBuffer(const MeshCreateInfo& mesh_info);
  void CreateIndexBuffer(const MeshCreateInfo& mesh_info);

private:
  VulkanContextData* context_;  

  VkBuffer vertex_buffer_;
  VkDeviceMemory vertex_buffer_memory_;
  VkBuffer index_buffer_;
  VkDeviceMemory index_buffer_memory_;
  uint32_t num_indices_ = 0;
};

}
