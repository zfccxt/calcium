#pragma once

#include <cstdint>
#include <memory>

#include "render_target.hpp"

namespace cl {

typedef uint32_t RenderPassLoadOpFlags;

constexpr RenderPassLoadOpFlags RenderPassLoadOp_Empty = 0;
constexpr RenderPassLoadOpFlags RenderPassLoadOp_Clear = 1 << 0;

struct RenderPassCreateInfo {
  RenderPassLoadOpFlags load_op_flags = RenderPassLoadOp_Clear;
  bool enable_depth_test = true;
  std::shared_ptr<RenderTarget> render_target;
};

}
