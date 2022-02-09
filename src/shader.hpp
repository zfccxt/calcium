#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "shader_reflection_details.hpp"

namespace cl {

class Shader {
public:
  virtual ~Shader() = default;

  virtual void Bind() = 0;

  BufferLayout GetInputLayout();
  
  static std::vector<uint32_t> ReadSPV(const char* file_path);

protected:
  ShaderReflectionDetails reflection_details_;
};

}
