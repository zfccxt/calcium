#pragma once

#include <cstdint>

namespace cl {

class Texture {
public:
  virtual ~Texture() = default;

  virtual size_t GetWidth() const = 0;
  virtual size_t GetHeight() const = 0;
};

}
