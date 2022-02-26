#pragma once

#include <cstdint>

#include "buffer_layout.hpp"

namespace cl {

struct MeshCreateInfo {
  BufferLayout vertex_input_layout;

  const float* vertices = nullptr;
  size_t num_vertices = 0;

  const uint32_t* indices = nullptr;
  size_t num_indices = 0;
};

}
