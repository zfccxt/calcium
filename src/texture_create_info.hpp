#pragma once

#include <string>

#include "texture_filter.hpp"
#include "texture_wrap.hpp"

namespace cl {

struct TextureCreateInfo {
  std::string file_path;
  TextureFilter filter = TextureFilter::kLinear;
  TextureWrap wrap = TextureWrap::kRepeat;
};

}
