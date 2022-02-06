#include "opengl_shader.hpp"

#include "shader_details.hpp"

namespace cl::OpenGL {

OpenGLShader::OpenGLShader(const ShaderCreateInfo& shader_info) {
  ShaderCodeMap code_map;
  if (!shader_info.vert_path.empty()) { code_map.emplace(ShaderType::kVertexShader,      ReadSPV(shader_info.vert_path.c_str())); }
  if (!shader_info.frag_path.empty()) { code_map.emplace(ShaderType::kFragmentShader,    ReadSPV(shader_info.frag_path.c_str())); }
  if (!shader_info.geom_path.empty()) { code_map.emplace(ShaderType::kGeometryShader,    ReadSPV(shader_info.geom_path.c_str())); }
  if (!shader_info.mesh_path.empty()) { code_map.emplace(ShaderType::kMeshShader,        ReadSPV(shader_info.mesh_path.c_str())); }
  if (!shader_info.task_path.empty()) { code_map.emplace(ShaderType::kTaskShader,        ReadSPV(shader_info.task_path.c_str())); }
  if (!shader_info.tess_path.empty()) { code_map.emplace(ShaderType::kTesselationShader, ReadSPV(shader_info.tess_path.c_str())); }
  if (!shader_info.comp_path.empty()) { code_map.emplace(ShaderType::kComputeShader,     ReadSPV(shader_info.comp_path.c_str())); }

  ShaderDetails reflection_details(code_map);

  // TODO
}

OpenGLShader::~OpenGLShader() {
  // glDeleteProgram(program_id_);
}

void OpenGLShader::Bind() {
  glUseProgram(program_id_);
}

}
