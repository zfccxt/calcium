#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "buffer_layout.hpp"
#include "shader_data_type.hpp"
#include "shader_stage.hpp"

namespace cl {

typedef std::unordered_map<ShaderStage, std::vector<uint32_t>> ShaderCodeMap;

struct ShaderReflectionDetails {
  void Reflect(const ShaderCodeMap& shader_code);

  BufferLayout vertex_input_layout;
 
  struct UniformData {
    std::string name;
    size_t binding;
    size_t size;
    ShaderStage stage;
  };
  std::vector<UniformData> uniforms;
};

}
