#pragma once

#include <cstddef>
#include <string>

#include "colour.hpp"
#include "winding_order.hpp"

namespace cl {

struct WindowCreateInfo {
  size_t width = 1280;
  size_t height = 720;
  std::string title;
  bool center = true;
  bool enable_backface_cull = true;
  WindingOrder front_face = WindingOrder::kClockwise;
  bool enable_depth_test = true; // TODO: fully implement this
  bool enable_resize = true;
  bool enable_vsync = true;
  Colour clear_colour;
};

}
