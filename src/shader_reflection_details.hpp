#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "buffer_layout.hpp"
#include "shader_data_type.hpp"
#include "shader_stage.hpp"

// TODO: Convert all single uniforms to uniform buffers?

namespace cl {

typedef std::vector<uint32_t> SpvCode;
typedef std::unordered_map<ShaderStage, SpvCode> ShaderCodeMap;

struct ShaderReflectionDetails {
  void Reflect(const ShaderCodeMap& shader_code);

  BufferLayout vertex_input_layout;
 
  struct UniformData {
    std::string name;
    std::string uniform_block_name;
    size_t binding;
    size_t size;
    ShaderStage stage;
  };
  std::unordered_map<size_t, UniformData> uniforms;

  struct TextureData {
    std::string name;
    size_t binding;
    ShaderStage stage;
  };
  std::unordered_map<size_t, TextureData> textures;

public:
  bool HasUniformsOrTextures() const {
    return !uniforms.empty() || !textures.empty();
  }
};

}
