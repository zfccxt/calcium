#pragma once

#include <cstddef>
#include <string>

namespace cl {

struct WindowCreateInfo {
  size_t width = 1280;
  size_t height = 720;
  std::string title;
};

}
