#pragma once

#include <string>
#include <vector>

#include "texture_filter.hpp"
#include "texture_wrap.hpp"

namespace cl {

struct TextureArrayCreateInfo {
  struct FileDetails {
    std::string file_path;
    bool flip_vertical_on_load = false;
  };
  std::vector<FileDetails> files;

  TextureFilter filter = TextureFilter::kLinear;
  TextureWrap wrap = TextureWrap::kRepeat;
};

}
