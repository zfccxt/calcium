#include "opengl_texture.hpp"

#include <assert.h>

#include <glad/glad.h>
#include <stb_image.h>

#include "instrumentor.hpp"
#include "opengl/opengl_check.hpp"

namespace cl::opengl {

GLenum OpenGLTexture::TextureFilterToGLMinFilter(TextureFilter filter) {
  switch (filter) {
    case TextureFilter::kLinear:  return GL_LINEAR_MIPMAP_LINEAR;
    case TextureFilter::kNearest: return GL_NEAREST;
    default:                      return GL_DONT_CARE;
  } 
}
GLenum OpenGLTexture::TextureFilterToGLMagFilter(TextureFilter filter) {
  switch (filter) {
    case TextureFilter::kLinear:  return GL_LINEAR;
    case TextureFilter::kNearest: return GL_NEAREST;
    default:                      return GL_DONT_CARE;
  }
}

GLenum OpenGLTexture::TextureWrapModeToGLEnum(TextureWrap wrap_mode) {
  switch (wrap_mode) {
    case TextureWrap::kRepeat:         return GL_REPEAT;
    case TextureWrap::kMirroredRepeat: return GL_MIRRORED_REPEAT;
    case TextureWrap::kClampToEdge:    return GL_CLAMP_TO_EDGE;
    case TextureWrap::kClampToBorder:  return GL_CLAMP_TO_BORDER;
    default:                           return GL_DONT_CARE;
  }
}

OpenGLTexture::OpenGLTexture(const TextureCreateInfo& texture_info) {
  CALCIUM_PROFILE_FUNCTION();

  int width, height, channels;
	stbi_uc* data = nullptr;

  stbi_set_flip_vertically_on_load(texture_info.flip_vertical_on_load);
	data = stbi_load(texture_info.file_path.c_str(), &width, &height, &channels, 0);

  width_ = width;
  height_ = height;

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
	
	GL_CHECK(glGenTextures(1, &texture_id_));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id_));

	GLenum wrap = TextureWrapModeToGLEnum(texture_info.wrap);
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToGLMinFilter(texture_info.filter)));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToGLMagFilter(texture_info.filter)));

	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data));
	stbi_image_free(data);

	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
}

OpenGLTexture::OpenGLTexture(const BlankTextureCreateInfo& texture_info) {
  CALCIUM_PROFILE_FUNCTION();

  width_ = texture_info.width;
  height_ = texture_info.height;

  GL_CHECK(glGenTextures(1, &texture_id_));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id_));

	GLenum wrap = TextureWrapModeToGLEnum(texture_info.wrap);
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToGLMinFilter(texture_info.filter)));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToGLMagFilter(texture_info.filter)));
	
	size_t data_size = (size_t)texture_info.width * texture_info.height;
	uint32_t* data = new uint32_t[data_size];
	// TODO: Why is this backwards?
	uint32_t colour = texture_info.colour.UintABGR();
	for (size_t i = 0; i < data_size; ++i) {
		data[i] = colour;
	}
	
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_info.width, texture_info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	delete[] data;

	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
}

OpenGLTexture::~OpenGLTexture() {
  CALCIUM_PROFILE_FUNCTION();

  GL_CHECK(glDeleteTextures(1, &texture_id_));
}

void OpenGLTexture::Bind() const {
  CALCIUM_PROFILE_FUNCTION();

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture_id_));
}

size_t OpenGLTexture::GetWidth() const {
  return width_;
}

size_t OpenGLTexture::GetHeight() const {
  return height_;
}

}
