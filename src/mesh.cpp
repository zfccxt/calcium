#include "mesh.hpp"

#include <fast_obj.h>
#include <meshoptimizer.h>

namespace cl {

MeshCreateInfo Mesh::LoadObj(const std::string& file_path) {
  fastObjMesh* mesh = fast_obj_read(file_path.c_str());
  assert(mesh);

  bool has_texcoords = mesh->texcoord_count > 1;
  bool has_normals = mesh->normal_count > 1;

  MeshCreateInfo mesh_info;
  if (!has_texcoords && !has_normals) {
    mesh_info.vertex_data_layout = { ShaderDataType::kFloat3 };
  }
  else if (has_texcoords && !has_normals) {
    mesh_info.vertex_data_layout = { ShaderDataType::kFloat3, ShaderDataType::kFloat2 };
  }
  else if (!has_texcoords && has_normals) {
    mesh_info.vertex_data_layout = { ShaderDataType::kFloat3, ShaderDataType::kFloat3 };
  }
  else {
    mesh_info.vertex_data_layout = { ShaderDataType::kFloat3, ShaderDataType::kFloat2, ShaderDataType::kFloat3 };
  }

  uint32_t vertex_size_bytes = mesh_info.vertex_data_layout.GetSize();
  uint32_t vertex_size_floats = vertex_size_bytes / sizeof(float);
  uint32_t unoptimised_vertex_data_size_floats = mesh->face_count * 3ULL * vertex_size_floats;

  std::vector<float> unindexed_vertex_data(unoptimised_vertex_data_size_floats);

  for (size_t i = 1; i <= mesh->face_count; ++i) {
    for (size_t j = 0; j < 3; ++j) {
      size_t current_vertex = (i - 1L) * 3L + j;
      size_t vertex_offset = current_vertex * vertex_size_floats;

      unindexed_vertex_data[vertex_offset++] = mesh->positions[mesh->indices[current_vertex].p * 3L + 0L];
      unindexed_vertex_data[vertex_offset++] = mesh->positions[mesh->indices[current_vertex].p * 3L + 1L];
      unindexed_vertex_data[vertex_offset++] = mesh->positions[mesh->indices[current_vertex].p * 3L + 2L];

      if (has_texcoords) {
        unindexed_vertex_data[vertex_offset++] = mesh->texcoords[mesh->indices[current_vertex].t * 2L + 0L];
        unindexed_vertex_data[vertex_offset++] = mesh->texcoords[mesh->indices[current_vertex].t * 2L + 1L];
      }

      if (has_normals) {
        unindexed_vertex_data[vertex_offset++] = mesh->normals[mesh->indices[current_vertex].n * 3L + 0L];
        unindexed_vertex_data[vertex_offset++] = mesh->normals[mesh->indices[current_vertex].n * 3L + 1L];
        unindexed_vertex_data[vertex_offset++] = mesh->normals[mesh->indices[current_vertex].n * 3L + 2L];
      }
    }
  }

  size_t index_count = mesh->face_count * 3ULL;
  std::vector<uint32_t> remap(index_count);
  size_t vertex_count = meshopt_generateVertexRemap(&remap[0], nullptr, index_count, &unindexed_vertex_data[0], index_count, vertex_size_bytes);

  std::vector<uint32_t> indices(index_count);
  mesh_info.vertex_data.resize(vertex_count * vertex_size_floats);

  meshopt_remapIndexBuffer(&indices[0], nullptr, index_count, &remap[0]);
  meshopt_remapVertexBuffer(&mesh_info.vertex_data[0], &unindexed_vertex_data[0], index_count, vertex_size_bytes, &remap[0]);

  // TODO: Test other meshoptimizer features and see if they improve performance:
  meshopt_optimizeVertexCache(&indices[0], &indices[0], index_count, vertex_count);
  meshopt_optimizeOverdraw(&indices[0], &indices[0], index_count, &mesh_info.vertex_data[0], vertex_count, vertex_size_bytes, 1.05f);
  meshopt_optimizeVertexFetch(&mesh_info.vertex_data[0], &indices[0], index_count, &mesh_info.vertex_data[0], vertex_count, vertex_size_bytes);

  fast_obj_destroy(mesh);
  
  // meshoptimizer only uses 4 byte index buffers, so convert the resulting buffer to 2 bytes per index before creating the mesh
  mesh_info.indices.resize(index_count);
  for (size_t i = 0; i < index_count; ++i) {
    mesh_info.indices[i] = (uint16_t)indices[i];
  }

  return mesh_info;
}

}
