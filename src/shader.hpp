#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include "shader_reflection_details.hpp"

namespace cl {

class Shader {
public:
  virtual ~Shader() = default;

  virtual void Bind() = 0;

  virtual void UploadUniform(int binding, void* data) = 0;
  virtual void UploadUniform(const std::string& name, void* data);

  BufferLayout GetInputLayout();
  
  static std::vector<uint32_t> ReadSPV(const char* file_path);

protected:
  ShaderReflectionDetails reflection_details_;
};

}
