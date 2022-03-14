#include "opengl_shader_utils.hpp"

#include <assert.h>
#include <stdio.h>
#include <vector>

#include "instrumentor.hpp"
#include "opengl/opengl_check.hpp"

namespace cl::opengl {

GLenum FindOpenGLShaderStage(ShaderStage stage) {
  switch (stage) {
    case ShaderStage::kComputeShader:  return GL_COMPUTE_SHADER;
    case ShaderStage::kVertexShader:   return GL_VERTEX_SHADER;
    case ShaderStage::kFragmentShader: return GL_FRAGMENT_SHADER;
    case ShaderStage::kGeometryShader: return GL_GEOMETRY_SHADER;
    default: assert(false); return -1;

    /* TODO: figure this out
    case ShaderStage::kMeshShader:        return GL_MESH_SHADER ??
    case ShaderStage::kTaskShader:        return GL_TASK_SHADER ??
    case ShaderStage::kTesselationShader: return GL_TESSELATION_SHADER ??
    */
  }
}

#ifdef CALCIUM_BUILD_VERBOSE
const char* GetShaderStageName(ShaderStage stage) {
  switch (stage) {
    case ShaderStage::kComputeShader:     return "Compute Shader";
    case ShaderStage::kVertexShader:      return "Vertex Shader";
    case ShaderStage::kFragmentShader:    return "Fragment Shader";
    case ShaderStage::kGeometryShader:    return "Geometry Shader";
    case ShaderStage::kMeshShader:        return "Mesh Shader";
    case ShaderStage::kTaskShader:        return "Task Shader";
    case ShaderStage::kTesselationShader: return "Tesselation Shader";
  }
}
#endif

GLuint CreateShaderModule(const std::string& glsl_code, ShaderStage shader_type) {
  CALCIUM_PROFILE_FUNCTION();

  GLuint shader;
  GL_CHECK(shader = glCreateShader(FindOpenGLShaderStage(shader_type)));
	const GLchar* source_ptr = (const GLchar*)glsl_code.c_str();

#ifdef CALCIUM_BUILD_VERBOSE
  printf("Compiling %s:\n%s\n", GetShaderStageName(shader_type), source_ptr);
#endif

  GL_CHECK(glShaderSource(shader, 1, &source_ptr, 0));
  GL_CHECK(glCompileShader(shader));

#ifdef CALCIUM_BUILD_DEBUG
  GLint is_compiled = 0;
  GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled));
  if (is_compiled == GL_FALSE) {
    GLint max_length = 0;
	  GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length));

	  // max_length includes the null character
	  std::vector<GLchar> info_log(max_length);
	  GL_CHECK(glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]));
	  GL_CHECK(glDeleteShader(shader));

	  fprintf(stderr, "Failed to compile shader: %s", info_log.data());
		assert(false);
	  return -1;
  }
#endif

  return shader;
}

}
