#include "shader.hpp"

#include <cstdint>
#include <assert.h>
#include <fstream>

namespace cl {

std::vector<uint32_t> Shader::ReadSPV(const char* file_path) {
  std::ifstream file(file_path, std::ios::ate | std::ios::binary);
  assert(file.is_open());
  size_t file_size = (size_t)file.tellg();
  std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));
  file.seekg(0);
  file.read((char*)buffer.data(), file_size);
  file.close();
  return buffer;
}

BufferLayout Shader::GetInputLayout() {
  return reflection_details_.vertex_input_layout;
}

void Shader::UploadUniform(const std::string& name, void* data) {
  for (const auto& uniform : reflection_details_.uniforms) {
    if (uniform.second.name == name) {
      UploadUniform(uniform.second.binding, data);
    }
  }
}

void Shader::BindTexture(const std::string& name, const std::shared_ptr<Texture>& texture) {
  for (const auto& tex : reflection_details_.textures) {
    if (tex.second.name == name) {
      BindTexture(tex.second.binding, texture);
    }
  }
}
}
