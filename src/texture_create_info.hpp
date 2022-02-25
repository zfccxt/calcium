#pragma once

#include <string>

#include "colour.hpp"
#include "texture_filter.hpp"
#include "texture_wrap.hpp"

namespace cl {

// TODO: Name these better

struct TextureCreateInfo {
  std::string file_path;
  TextureFilter filter = TextureFilter::kLinear;
  TextureWrap wrap = TextureWrap::kRepeat;
};

struct BlankTextureCreateInfo {
  Colour colour = 0xd7de73ff;
  size_t width = 1;
  size_t height = 1;
  TextureFilter filter = TextureFilter::kNearest;
  TextureWrap wrap = TextureWrap::kRepeat;
};

}
