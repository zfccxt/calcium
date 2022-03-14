#include "opengl_texture_array.hpp"

#include <assert.h>

#include "instrumentor.hpp"
#include "texture_utils.hpp"
#include "opengl/opengl_check.hpp"
#include "opengl/opengl_texture.hpp"

namespace cl::opengl {

OpenGLTextureArray::OpenGLTextureArray(const TextureArrayCreateInfo& texture_array_info) {
  CALCIUM_PROFILE_FUNCTION();

  auto raw_arr = LoadRawTextureArray(texture_array_info);

  width_ = raw_arr->width;
  height_ = raw_arr->height;
  depth_ = raw_arr->depth;

  // Determine storage format based on loaded texture properties
	GLenum internal_format = 0, data_format = 0;
  
	if (raw_arr->channels == 4) {
		internal_format = GL_RGBA8;
		data_format = GL_RGBA;
	}
	else if (raw_arr->channels == 3) {
		internal_format = GL_RGB8;
		data_format = GL_RGB;
	}
	assert(internal_format & data_format);

  // Texture array data is loaded - now upload it to the GPU
  GL_CHECK(glGenTextures(1, &texture_id_));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_));
  // Upload pixel data.
  GL_CHECK(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, raw_arr->width, raw_arr->height, raw_arr->depth, 0, data_format, GL_UNSIGNED_BYTE, raw_arr->data));
  
  // Always set reasonable texture parameters
	GLenum wrap = OpenGLTexture::TextureWrapModeToGLEnum(texture_array_info.wrap);
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap));

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, OpenGLTexture::TextureFilterToGLMinFilter(texture_array_info.filter)));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, OpenGLTexture::TextureFilterToGLMagFilter(texture_array_info.filter)));	

	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D_ARRAY));
}

OpenGLTextureArray::OpenGLTextureArray(const BlankTextureCreateInfo& blank_texture_info) {
  size_t data_size = (size_t)blank_texture_info.width * blank_texture_info.height;
	uint32_t* data = new uint32_t[data_size];
	// TODO: Why is this backwards?
	uint32_t colour = blank_texture_info.colour.UintABGR();
	for (size_t i = 0; i < data_size; ++i) {
		data[i] = colour;
	}

  width_ = blank_texture_info.width;
  height_ = blank_texture_info.height;
  depth_ = 1;

  // Texture array data is loaded - now upload it to the GPU
  GL_CHECK(glGenTextures(1, &texture_id_));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_));
  // Upload pixel data.
  GL_CHECK(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, blank_texture_info.width, blank_texture_info.height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
  
  // Always set reasonable texture parameters
	GLenum wrap = OpenGLTexture::TextureWrapModeToGLEnum(blank_texture_info.wrap);
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap));

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, OpenGLTexture::TextureFilterToGLMinFilter(blank_texture_info.filter)));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, OpenGLTexture::TextureFilterToGLMagFilter(blank_texture_info.filter)));	

  delete[] data;
	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D_ARRAY));
}

OpenGLTextureArray::~OpenGLTextureArray() {
  CALCIUM_PROFILE_FUNCTION();

  GL_CHECK(glDeleteTextures(1, &texture_id_));
}

void OpenGLTextureArray::Bind() const {
  CALCIUM_PROFILE_FUNCTION();

  GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_));
}

size_t OpenGLTextureArray::GetWidth() const {
  return width_;
}

size_t OpenGLTextureArray::GetHeight() const {
  return height_;
}

size_t OpenGLTextureArray::GetDepth() const {
  return depth_;
}

}
