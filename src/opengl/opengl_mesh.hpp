#pragma once

#include "mesh.hpp"
#include "mesh_create_info.hpp"

namespace cl::OpenGL {

class OpenGLMesh : public Mesh {
public:
  OpenGLMesh(const MeshCreateInfo& file_path);

  virtual void Bind() override;
};

}
