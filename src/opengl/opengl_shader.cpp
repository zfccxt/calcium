#include "opengl_shader.hpp"

#include <glad/glad.h>
// spirv-cross is also included with the Vulkan SDK. Make sure not to #include <spirv-cross/spirv_xxx> as this will
// cause mismatched header and cpp files and you will end up with linker errors
#include <spirv_glsl.hpp>

#include "opengl_shader_utils.hpp"
#include "shader_reflection_details.hpp"

namespace cl::OpenGL {

OpenGLShader::OpenGLShader(const ShaderCreateInfo& shader_info) {
  ShaderCodeMap code_map;
  if (!shader_info.vert_path.empty()) { code_map.emplace(ShaderStage::kVertexShader,      ReadSPV(shader_info.vert_path.c_str())); }
  if (!shader_info.frag_path.empty()) { code_map.emplace(ShaderStage::kFragmentShader,    ReadSPV(shader_info.frag_path.c_str())); }
  if (!shader_info.geom_path.empty()) { code_map.emplace(ShaderStage::kGeometryShader,    ReadSPV(shader_info.geom_path.c_str())); }
  if (!shader_info.mesh_path.empty()) { code_map.emplace(ShaderStage::kMeshShader,        ReadSPV(shader_info.mesh_path.c_str())); }
  if (!shader_info.task_path.empty()) { code_map.emplace(ShaderStage::kTaskShader,        ReadSPV(shader_info.task_path.c_str())); }
  if (!shader_info.tess_path.empty()) { code_map.emplace(ShaderStage::kTesselationShader, ReadSPV(shader_info.tess_path.c_str())); }
  if (!shader_info.comp_path.empty()) { code_map.emplace(ShaderStage::kComputeShader,     ReadSPV(shader_info.comp_path.c_str())); }

  reflection_details_.Reflect(code_map);

  program_id_ = glCreateProgram();
  std::vector<GLuint> shader_modules;
  for (const auto& spv : code_map) {
    spirv_cross::CompilerGLSL compiler(spv.second);
    const std::string glsl_code = compiler.compile();
		GLuint shader_module = CreateShaderModule(glsl_code, spv.first);
    shader_modules.push_back(shader_module);
	  glAttachShader(program_id_, shader_module);
  }
  glLinkProgram(program_id_);

  // Check link status
#ifdef CALCIUM_BUILD_DEBUG
	GLint is_linked = 0;
	glGetProgramiv(program_id_, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE) {
		GLint max_length = 0;
		glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &max_length);
		std::vector<GLchar> info_log(max_length);
		glGetProgramInfoLog(program_id_, max_length, &max_length, &info_log[0]);
	
		glDeleteProgram(program_id_);
		for (auto shader : shader_modules) {
			glDeleteShader(shader);
		}

	  fprintf(stderr, "Failed to link shader: %s\n", info_log.data());
		assert(false);
		return;
	}
#endif

	// Always detach shaders after a successful link.
	for (auto shader : shader_modules) {
    glDetachShader(program_id_, shader);
    // TODO: check if this is okay
    glDeleteShader(shader);
	}
}

OpenGLShader::~OpenGLShader() {
  glDeleteProgram(program_id_);
}

void OpenGLShader::Bind() {
  glUseProgram(program_id_);
}

}
