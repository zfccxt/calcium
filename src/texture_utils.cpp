#include "texture_utils.hpp"

#include <assert.h>

#include <glad/glad.h>
#include <stb_image.h>

namespace cl {

std::unique_ptr<RawTextureArray> LoadRawTextureArray(const TextureArrayCreateInfo& texture_array_info) {
  auto raw_arr = std::make_unique<RawTextureArray>(); 

  assert(texture_array_info.files.size() > 0);

  // Load first texture to find the width and height of the texture array
  raw_arr->depth = texture_array_info.files.size();

	stbi_uc* first_tex_data = nullptr;
  stbi_set_flip_vertically_on_load(texture_array_info.files[0].flip_vertical_on_load);
	first_tex_data = stbi_load(texture_array_info.files[0].file_path.c_str(), &raw_arr->width, &raw_arr->height, &raw_arr->channels, 0);
	assert(first_tex_data);

  GLsizei texture_size_bytes = (raw_arr->width * raw_arr->height * raw_arr->channels);

  // Assign temporary cpu side storage buffer for textures
  raw_arr->data = new char[texture_size_bytes * raw_arr->depth];
  // Copy first loaded texture into buffer
  memcpy(raw_arr->data, first_tex_data, texture_size_bytes);

  // First texture has been copied into the buffer that we will upload to the GPU, so it can now be freed
  stbi_image_free(first_tex_data);

  // For each texture after the first
  for (size_t i = 1; i < raw_arr->depth; ++i) {
    // Load the texture into cpu memory
    stbi_uc* next_tex_data = nullptr;
    stbi_set_flip_vertically_on_load(texture_array_info.files[i].flip_vertical_on_load);

    // Unused
    int w, h, c;
	  next_tex_data = stbi_load(texture_array_info.files[i].file_path.c_str(), &w, &h, &c, 0);
	  assert(next_tex_data);

    // Copy it into the buffer with the other textures in this array
    memcpy(raw_arr->data + i * texture_size_bytes, next_tex_data, texture_size_bytes);
    stbi_image_free(next_tex_data);
  }

  return raw_arr;
}

}
