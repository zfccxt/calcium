#pragma once

#include <memory>

#include "texture_array_create_info.hpp"

namespace cl {

struct RawTextureArray {
  char* data;
  int width;
  int height;
  int depth;
  int channels;

  ~RawTextureArray() { delete[] data; }
};

std::unique_ptr<RawTextureArray> LoadRawTextureArray(const TextureArrayCreateInfo& texture_array_info);

}
