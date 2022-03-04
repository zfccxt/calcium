#pragma once

#include <glad/glad.h>

#include "mesh.hpp"
#include "mesh_create_info.hpp"

namespace cl::opengl {

class OpenGLMesh : public Mesh {
public:
  OpenGLMesh(const MeshCreateInfo& mesh_info);
  ~OpenGLMesh();

  virtual void Draw() override;

private:
  GLuint vertex_array_id_;
  GLuint vertex_buffer_id_;
  GLuint index_buffer_id_;
  GLsizei num_indices_;
};

}
