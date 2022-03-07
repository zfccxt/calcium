#pragma once

#include <cstdint>
#include <vector>

#include "buffer_layout.hpp"

namespace cl {

struct MeshCreateInfo {
  BufferLayout vertex_input_layout;
  std::vector<float> vertices;
  std::vector<uint32_t> indices;
};

}
