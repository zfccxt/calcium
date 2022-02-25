#pragma once

#include <cstdint>

#include "buffer_layout.hpp"

namespace cl {

struct MeshCreateInfo {
  BufferLayout vertex_data_layout;

  float* vertices = nullptr;
  size_t num_vertices = 0;

  uint32_t* indices = nullptr;
  size_t num_indices = 0;
};

}
