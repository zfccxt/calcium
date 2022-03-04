#include "opengl_shader_utils.hpp"

#include <assert.h>
#include <stdio.h>
#include <vector>

#include "instrumentor.hpp"

namespace cl::opengl {

GLenum FindOpenGLShaderStage(ShaderStage type) {
  switch (type) {
    case ShaderStage::kComputeShader:  return GL_COMPUTE_SHADER;
    case ShaderStage::kVertexShader:   return GL_VERTEX_SHADER;
    case ShaderStage::kFragmentShader: return GL_FRAGMENT_SHADER;
    case ShaderStage::kGeometryShader: return GL_GEOMETRY_SHADER;
    default: assert(false); return -1;

    /* TODO: figure this out
    case ShaderType::kMeshShader:        return GL_MESH_SHADER ??
    case ShaderType::kTaskShader:        return GL_TASK_SHADER ??
    case ShaderType::kTesselationShader: return GL_TESSELATION_SHADER ??
    */
  }
}

GLuint CreateShaderModule(const std::string& glsl_code, ShaderStage shader_type) {
  CALCIUM_PROFILE_FUNCTION();

  GLuint shader = glCreateShader(FindOpenGLShaderStage(shader_type));
	const GLchar* source_ptr = (const GLchar*)glsl_code.c_str();
  glShaderSource(shader, 1, &source_ptr, 0);
  glCompileShader(shader);

#ifdef CALCIUM_BUILD_DEBUG
  GLint is_compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    GLint max_length = 0;
	  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

	  // max_length includes the null character
	  std::vector<GLchar> info_log(max_length);
	  glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);
	  glDeleteShader(shader);

	  fprintf(stderr, "Failed to compile shader: %s", info_log.data());
		assert(false);
	  return -1;
  }
#endif

  return shader;
}

}
