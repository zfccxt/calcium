#pragma once

#include <cstdint>

namespace cl {

struct Colour {
  Colour() = default;
  Colour(float r, float g, float b, float a);
  Colour(uint32_t colour);

  uint32_t UintRGBA() const;
  uint32_t UintABGR() const;

  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 1.0f;

private:
  void SetColour(uint32_t colour);
};

}
