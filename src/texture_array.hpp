#pragma once

#include <cstdint>

namespace cl {

class TextureArray {
public:
  virtual ~TextureArray() = default;

  virtual size_t GetWidth() const = 0;
  virtual size_t GetHeight() const = 0;
  virtual size_t GetDepth() const = 0;
};

}
