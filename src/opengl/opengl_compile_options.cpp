#include "opengl_compile_options.hpp"

#include <glad/glad.h>

#include "instrumentor.hpp"

namespace cl::OpenGL {

void ApplyOpenGLCompileOptions(spirv_cross::CompilerGLSL& compiler) {
  CALCIUM_PROFILE_FUNCTION();

	spirv_cross::CompilerGLSL::Options options;
  
  // On emscripten, target GLES 3.0
#ifdef CALCIUM_EMSCRIPTEN
	options.version = 300;
	options.es = true;
#endif

	// On desktop platforms, use the highest supported GLSL version and allow use of any extensions that are supported.
#ifndef CALCIUM_EMSCRIPTEN
  // Interrogate the current context's GLSL version and convert it to an int
  // We want to use the most up to date GLSL version possible to make use of modern features
  // This also helps have minimal issues converting shaders that use Vulkan semantics
  std::string glsl_version_str = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
  int version_major, version_minor;
	// This is a stupid hack but guaranteed to work by the spec
  // If you don't believe me look it up https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetString.xhtml
  (void)sscanf(glsl_version_str.c_str(), "%d%*c%d", &version_major, &version_minor);
  options.version = version_major * 100 + version_minor;
#endif

  // macOS only supports OpenGL versions up to 4.1 due to OpenGL being deprecated on the platform.
  // Unfortunately, explicit binding points are a 4.2 feature. Therefore we have to tell SPIRV-Cross
  // not to generate binding points for uniform buffers on macOS and other platforms that don't support OpenGL
	// versions 4.2 and later.
	// Unsure why SPIRV-Cross doesn't set this option for you automatically. Maybe it's an oversight. 
	if (options.version < 420) {
		options.enable_420pack_extension = false;
	}

	// We want to use the Vulkan style normalised device coordinate system for all rendering backends
	// The easiest place to apply that option is here, while cross-compiling the shader
	// We tell SPIRV-Cross to insert an additional gl_Position.y = -gl_Position.y at the end of the vertex shader
	options.vertex.flip_vert_y = true;

	compiler.set_common_options(options);
}

}
