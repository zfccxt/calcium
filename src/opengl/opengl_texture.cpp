#include "opengl_texture.hpp"

#include <assert.h>

#include <glad/glad.h>
#include <stb_image.h>

namespace cl::OpenGL {

static GLenum TextureFilterToGLEnum(TextureFilter filter) {
  switch (filter) {
    case TextureFilter::kLinear:  return GL_LINEAR;
    case TextureFilter::kNearest: return GL_NEAREST;
    default:                      return GL_DONT_CARE;
  }
}

static GLenum TextureWrapModeToGLEnum(TextureWrap wrap_mode) {
  switch (wrap_mode) {
    case TextureWrap::kRepeat:         return GL_REPEAT;
    case TextureWrap::kMirroredRepeat: return GL_MIRRORED_REPEAT;
    case TextureWrap::kClampToEdge:    return GL_CLAMP_TO_EDGE;
    case TextureWrap::kClampToBorder:  return GL_CLAMP_TO_BORDER;
    default:                           return GL_DONT_CARE;
  }
}

OpenGLTexture::OpenGLTexture(const TextureCreateInfo& texture_info) {
  int width, height, channels;
	stbi_uc* data = nullptr;

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(texture_info.file_path.c_str(), &width, &height, &channels, 0);

	assert(data);

	GLenum internal_format = 0, data_format = 0;

	if (channels == 4) {
		internal_format = GL_RGBA8;
		data_format = GL_RGBA;
	}
	else if (channels == 3) {
		internal_format = GL_RGB8;
		data_format = GL_RGB;
	}
	
	assert(internal_format & data_format);
	
	glGenTextures(1, &texture_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	
	GLenum filter = TextureFilterToGLEnum(texture_info.filter);
	glTexParameteri(texture_id_, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(texture_id_, GL_TEXTURE_MAG_FILTER, filter);
	
	GLenum wrap = TextureWrapModeToGLEnum(texture_info.wrap);
	glTexParameteri(texture_id_, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(texture_id_, GL_TEXTURE_WRAP_T, wrap);
	
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(data);
}

}
