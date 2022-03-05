#include "shader.hpp"

#include <cstdint>
#include <assert.h>
#include <fstream>

#include "instrumentor.hpp"

namespace cl {

SpvCode Shader::ReadSpv(const char* file_path) {
  CALCIUM_PROFILE_FUNCTION();

  std::ifstream file(file_path, std::ios::ate | std::ios::binary);
  assert(file.is_open());
  size_t file_size = (size_t)file.tellg();
  SpvCode buffer(file_size / sizeof(uint32_t));
  file.seekg(0);
  file.read((char*)buffer.data(), file_size);
  file.close();
  return buffer;
}

void Shader::UploadUniform(const std::string& name, void* data) {
  for (const auto& uniform : reflection_details_.uniforms) {
    if (uniform.second.name == name) {
      UploadUniform(uniform.second.binding, data);
      return;
    }
  }
}

ShaderCodeMap Shader::ReadAllSpvFiles(const ShaderCreateInfo& shader_info) {
  CALCIUM_PROFILE_FUNCTION();

  ShaderCodeMap code_map;
  if (!shader_info.vert_path.empty()) { code_map.emplace(ShaderStage::kVertexShader,      ReadSpv(shader_info.vert_path.c_str())); }
  if (!shader_info.frag_path.empty()) { code_map.emplace(ShaderStage::kFragmentShader,    ReadSpv(shader_info.frag_path.c_str())); }
  if (!shader_info.geom_path.empty()) { code_map.emplace(ShaderStage::kGeometryShader,    ReadSpv(shader_info.geom_path.c_str())); }
  if (!shader_info.mesh_path.empty()) { code_map.emplace(ShaderStage::kMeshShader,        ReadSpv(shader_info.mesh_path.c_str())); }
  if (!shader_info.task_path.empty()) { code_map.emplace(ShaderStage::kTaskShader,        ReadSpv(shader_info.task_path.c_str())); }
  if (!shader_info.tess_path.empty()) { code_map.emplace(ShaderStage::kTesselationShader, ReadSpv(shader_info.tess_path.c_str())); }
  if (!shader_info.comp_path.empty()) { code_map.emplace(ShaderStage::kComputeShader,     ReadSpv(shader_info.comp_path.c_str())); }
  return code_map;
}

void Shader::BindTexture(const std::string& name, const std::shared_ptr<Texture>& texture) {
  for (const auto& tex : reflection_details_.textures) {
    if (tex.second.name == name) {
      BindTexture(tex.second.binding, texture);
      break;
    }
  }
}
}
