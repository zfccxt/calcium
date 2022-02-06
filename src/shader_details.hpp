#pragma once

#include <unordered_map>

#include "shader_type.hpp"

namespace cl {

typedef std::unordered_map<ShaderType, std::vector<uint32_t>> ShaderCodeMap;

struct ShaderDetails {
  ShaderDetails(const ShaderCodeMap& shaders);
};

}
