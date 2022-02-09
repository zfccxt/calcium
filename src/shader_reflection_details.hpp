#pragma once

#include <unordered_map>

#include "buffer_layout.hpp"
#include "shader_type.hpp"

namespace cl {

typedef std::unordered_map<ShaderType, std::vector<uint32_t>> ShaderCodeMap;

struct ShaderReflectionDetails {
  void Reflect(const ShaderCodeMap& shader_code);

  BufferLayout vertex_input_layout;
};

}
