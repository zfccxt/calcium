#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include "shader_create_info.hpp"
#include "shader_reflection_details.hpp"
#include "texture.hpp"

namespace cl {

class Shader {
public:
  virtual ~Shader() = default;

  virtual void Bind() = 0;

  virtual void UploadUniform(int binding, void* data) = 0;
  virtual void UploadUniform(const std::string& name, void* data);

  virtual void BindTexture(int binding, const std::shared_ptr<Texture>& texture) = 0;
  virtual void BindTexture(const std::string& name, const std::shared_ptr<Texture>& texture);

  BufferLayout GetInputLayout();
  
  static SpvCode ReadSpv(const char* file_path);
  static ShaderCodeMap ReadAllSpvFiles(const ShaderCreateInfo& shader_info);

protected:
  ShaderReflectionDetails reflection_details_;
};

}
