#include "opengl_mesh.hpp"

#include <assert.h>

#include "instrumentor.hpp"
#include "opengl/opengl_check.hpp"

namespace cl::opengl {

GLenum OpenGLTypeOf(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::kBool:    return GL_BOOL;
    case ShaderDataType::kInt:     return GL_INT;
    case ShaderDataType::kInt2:    return GL_INT;
    case ShaderDataType::kInt3:    return GL_INT;
    case ShaderDataType::kInt4:    return GL_INT;
    case ShaderDataType::kFloat:   return GL_FLOAT;
    case ShaderDataType::kFloat2:  return GL_FLOAT;
    case ShaderDataType::kFloat3:  return GL_FLOAT;
    case ShaderDataType::kFloat4:  return GL_FLOAT;
    case ShaderDataType::kMatrix3: return GL_FLOAT;
    case ShaderDataType::kMatrix4: return GL_FLOAT;
    default:                       return -1;
  }
}

OpenGLMesh::OpenGLMesh(const MeshCreateInfo& mesh_info) {
  CALCIUM_PROFILE_FUNCTION();

  GL_CHECK(glGenVertexArrays(1, &vertex_array_id_));
  GL_CHECK(glBindVertexArray(vertex_array_id_));

  // Make sure the buffer layout contains at least one element
  assert(mesh_info.vertex_input_layout.GetNumElements() > 0);

  // Create vertex buffer and upload vertex data to GPU
  // Calcium uses interleaved vertex data rather than separate buffers for positions, tex coords, normals, etc
  GL_CHECK(glGenBuffers(1, &vertex_buffer_id_));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, mesh_info.vertices.size() * sizeof(float), mesh_info.vertices.data(), GL_STATIC_DRAW));

  // For each element in the buffer layout, tell OpenGL how to interpret that data
  for (int i = 0; i < mesh_info.vertex_input_layout.GetNumElements(); ++i) {
    GL_CHECK(glEnableVertexAttribArray(i));
    GL_CHECK(glVertexAttribPointer(i, ComponentCountOf(mesh_info.vertex_input_layout[i]), 
      OpenGLTypeOf(mesh_info.vertex_input_layout[i]),
      GL_FALSE, 
      mesh_info.vertex_input_layout.GetStride(), 
      (const void*)(uint64_t)mesh_info.vertex_input_layout[i].offset));
  }

  // Create index buffer
  GL_CHECK(glGenBuffers(1, &index_buffer_id_));
  GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_));
  GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_info.indices.size() * sizeof(uint32_t), mesh_info.indices.data(), GL_STATIC_DRAW));
  num_indices_ = mesh_info.indices.size();

  // Unbind vao
  GL_CHECK(glBindVertexArray(0));
}

OpenGLMesh::~OpenGLMesh() {
  CALCIUM_PROFILE_FUNCTION();

  GL_CHECK(glDeleteBuffers(1, &index_buffer_id_));
  GL_CHECK(glDeleteBuffers(1, &vertex_buffer_id_));
  GL_CHECK(glDeleteVertexArrays(1, &vertex_array_id_));
}

void OpenGLMesh::Draw() {
  CALCIUM_PROFILE_FUNCTION();

  GL_CHECK(glBindVertexArray(vertex_array_id_));
  GL_CHECK(glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0));
}

}
