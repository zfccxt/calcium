#include "colour.hpp"

namespace cl {

Colour::Colour(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
}

Colour::Colour(uint32_t colour) { 
  SetColour(colour);
};

void Colour::SetColour(uint32_t colour) {
  int rr = (colour >> 24) & 0xff;
  int gg = (colour >> 16) & 0xff;
  int bb = (colour >> 8 ) & 0xff;
  int aa =  colour        & 0xff;

  r = rr / 255.0f;
  g = gg / 255.0f;
  b = bb / 255.0f;
  a = aa / 255.0f;
}

uint32_t Colour::UintRGBA() const {
  unsigned char rr = 0xff * r;
  unsigned char gg = 0xff * g;
  unsigned char bb = 0xff * b;
  unsigned char aa = 0xff * a;
  return  (rr << 24) | (gg << 16) | (bb << 8) | aa;
};

uint32_t Colour::UintABGR() const {
  unsigned char rr = 0xff * r;
  unsigned char gg = 0xff * g;
  unsigned char bb = 0xff * b;
  unsigned char aa = 0xff * a;
  return  (aa << 24) | (bb << 16) | (gg << 8) | rr;
};

}
