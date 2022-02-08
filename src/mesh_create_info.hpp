#pragma once

#include <cstdint>

#include "buffer_layout.hpp"

namespace cl {

struct MeshCreateInfo {
  BufferLayout vertex_data_layout;

  float* vertices;
  size_t num_vertices;

  uint32_t* indices;
  size_t num_indices;
};

}
