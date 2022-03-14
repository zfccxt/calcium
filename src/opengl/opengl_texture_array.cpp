#include "opengl_texture_array.hpp"

#include <assert.h>

#include <stb_image.h>

#include "instrumentor.hpp"
#include "opengl/opengl_check.hpp"
#include "opengl/opengl_texture.hpp"

namespace cl::opengl {

OpenGLTextureArray::OpenGLTextureArray(const TextureArrayCreateInfo& texture_array_info) {
  CALCIUM_PROFILE_FUNCTION();

  assert(texture_array_info.files.size() > 0);

  // Load first texture to find the width and height of the texture array
  int width, height, channels;
  GLsizei num_layers = texture_array_info.files.size();

	stbi_uc* first_tex_data = nullptr;
  stbi_set_flip_vertically_on_load(texture_array_info.files[0].flip_vertical_on_load);
	first_tex_data = stbi_load(texture_array_info.files[0].file_path.c_str(), &width, &height, &channels, 0);
	assert(first_tex_data);

  width_ = width;
  height_ = height;
  GLsizei texture_size_bytes = (width_ * height_ * channels);

  // Determine storage format based on loaded texture properties
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

  // Assign temporary cpu side storage buffer for textures
  stbi_uc* data = new stbi_uc[texture_size_bytes * num_layers];
  // Copy first loaded texture into buffer
  memcpy(data, first_tex_data, texture_size_bytes);

  // First texture has been copied into the buffer that we will upload to the GPU, so it can now be freed
  stbi_image_free(first_tex_data);

  // For each texture after the first
  for (size_t i = 1; i < num_layers; ++i) {
    // Load the texture into cpu memory
    stbi_uc* next_tex_data = nullptr;
    stbi_set_flip_vertically_on_load(texture_array_info.files[i].flip_vertical_on_load);

    // Unused
    int w, h, c;
	  next_tex_data = stbi_load(texture_array_info.files[i].file_path.c_str(), &w, &h, &c, 0);
	  assert(next_tex_data);

    // Copy it into the buffer with the other textures in this array
    memcpy(data + i * texture_size_bytes, next_tex_data, texture_size_bytes);
    stbi_image_free(next_tex_data);
  }

  // TODO: Figure out what the mip level count should be
  GLsizei mip_level_count = 1;

  // Texture array data is loaded - now upload it to the GPU
  GL_CHECK(glGenTextures(1, &texture_id_));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_));
  GL_CHECK(glTexStorage3D(GL_TEXTURE_2D_ARRAY, mip_level_count, internal_format, width, height, num_layers));
  // Upload pixel data.
  // The first 0 refers to the mipmap level (level 0, since there's only 1)
  // The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
  // The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
  // Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
  GL_CHECK(glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, num_layers, data_format, GL_UNSIGNED_BYTE, data));
  
  // Always set reasonable texture parameters
	GLenum wrap = OpenGLTexture::TextureWrapModeToGLEnum(texture_array_info.wrap);
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap));

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, OpenGLTexture::TextureFilterToGLMinFilter(texture_array_info.filter)));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, OpenGLTexture::TextureFilterToGLMagFilter(texture_array_info.filter)));	

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

}
