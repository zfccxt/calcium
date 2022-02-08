#pragma once

#include "mesh.hpp"
#include "mesh_create_info.hpp"

namespace cl::Vulkan {

class VulkanMesh : public Mesh {
public:
  VulkanMesh(const MeshCreateInfo& file_path);

  virtual void Bind() override;
};

}
