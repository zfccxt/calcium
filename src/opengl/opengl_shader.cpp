#include "opengl_shader.hpp"

#include <glad/glad.h>
// spirv-cross is also included with the Vulkan SDK. Make sure not to #include <spirv-cross/spirv_xxx> as this will
// cause mismatched header and cpp files and you will end up with linker errors
#include <spirv_glsl.hpp>

#include "instrumentor.hpp"
#include "opengl_check.hpp"
#include "opengl_compile_options.hpp"
#include "opengl_shader_utils.hpp"
#include "opengl_texture.hpp"
#include "opengl_texture_array.hpp"
#include "shader_reflection_details.hpp"

namespace cl::opengl {

OpenGLShader::OpenGLShader(const ShaderCreateInfo& shader_info) {
  CALCIUM_PROFILE_FUNCTION();

  ShaderCodeMap code_map = ReadAllSpvFiles(shader_info);
  reflection_details_.Reflect(code_map); 

  GL_CHECK(program_id_ = glCreateProgram());
  std::vector<GLuint> shader_modules;
  for (const auto& spv : code_map) {
    spirv_cross::CompilerGLSL compiler(spv.second);
    ApplyOpenGLCompileOptions(compiler);
    const std::string glsl_code = compiler.compile();
		GLuint shader_module = CreateShaderModule(glsl_code, spv.first);
    shader_modules.push_back(shader_module);
	  GL_CHECK(glAttachShader(program_id_, shader_module));
  }
  GL_CHECK(glLinkProgram(program_id_));

  // Check link status
#ifdef CALCIUM_BUILD_DEBUG
	GLint is_linked = 0;
	GL_CHECK(glGetProgramiv(program_id_, GL_LINK_STATUS, &is_linked));
	if (is_linked == GL_FALSE) {
		GLint max_length = 0;
		GL_CHECK(glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &max_length));
		std::vector<GLchar> info_log(max_length);
		GL_CHECK(glGetProgramInfoLog(program_id_, max_length, &max_length, &info_log[0]));
	
		GL_CHECK(glDeleteProgram(program_id_));
		for (auto shader : shader_modules) {
			GL_CHECK(glDeleteShader(shader));
		}

	  fprintf(stderr, "Failed to link shader: %s\n", info_log.data());
		assert(false);
		return;
	}
#endif

	// Always detach shaders after a successful link.
	for (auto shader : shader_modules) {
    GL_CHECK(glDetachShader(program_id_, shader));
    GL_CHECK(glDeleteShader(shader));
	}

  // Create uniform buffers
  for (const auto& uniform : reflection_details_.uniforms) {
    uniforms_.emplace(uniform.first, std::make_unique<OpenGLUniformBuffer>(program_id_, uniform.second.binding, uniform.second.size, uniform.second.uniform_block_name));
  }

  glUseProgram(program_id_);
  // Store sampler bindings
  size_t i = 0;
  for (const auto& sampler : reflection_details_.textures) {
    samplers_.emplace(sampler.first, i);
    GL_CHECK(glUniform1i(glGetUniformLocation(program_id_, sampler.second.name.c_str()), i));
    ++i;
  }
  for (const auto& sampler : reflection_details_.texture_arrays) {
    samplers_.emplace(sampler.first, i);
    GL_CHECK(glUniform1i(glGetUniformLocation(program_id_, sampler.second.name.c_str()), i));
    ++i;
  }
}

OpenGLShader::~OpenGLShader() {
  GL_CHECK(glDeleteProgram(program_id_));
}

void OpenGLShader::Bind() {
  GL_CHECK(glUseProgram(program_id_));
}

void OpenGLShader::UploadUniform(int binding, void* data) {
  uniforms_[binding]->Upload(data);
}

void OpenGLShader::BindTexture(int binding, const std::shared_ptr<Texture>& texture) {
  CALCIUM_PROFILE_FUNCTION();

  size_t slot = samplers_[binding];
  GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
  std::dynamic_pointer_cast<OpenGLTexture>(texture)->Bind();
}

void OpenGLShader::BindTextureArray(int binding, const std::shared_ptr<TextureArray>& texture) {
  CALCIUM_PROFILE_FUNCTION();

  size_t slot = samplers_[binding];
  GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
  std::dynamic_pointer_cast<OpenGLTextureArray>(texture)->Bind();
}

void OpenGLShader::BindAllTextureSamplers(const std::shared_ptr<Texture>& texture) {
  CALCIUM_PROFILE_FUNCTION();

  for (const auto& sampler : samplers_) {
    // If sampler binding point is a sampler2D
    if (reflection_details_.textures.find(sampler.first) != reflection_details_.textures.end()) {
      GL_CHECK(glActiveTexture(GL_TEXTURE0 + sampler.second));
      std::dynamic_pointer_cast<OpenGLTexture>(texture)->Bind();
    }
  }
}

void OpenGLShader::BindAllTextureArraySamplers(const std::shared_ptr<TextureArray>& texture_array) {
  CALCIUM_PROFILE_FUNCTION();

  for (const auto& sampler : samplers_) {
    // If sampler binding point is a sampler2D
    if (reflection_details_.texture_arrays.find(sampler.first) != reflection_details_.texture_arrays.end()) {
      GL_CHECK(glActiveTexture(GL_TEXTURE0 + sampler.second));
      std::dynamic_pointer_cast<OpenGLTextureArray>(texture_array)->Bind();
    }
  }
}

}
