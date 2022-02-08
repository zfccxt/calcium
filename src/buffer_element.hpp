#pragma once

#include <cstddef>

#include "shader_data_type.hpp"

namespace cl {

// A wrapper around a ShaderDataType, caching size and offset
// To be used by BufferLayout
struct BufferElement {
  BufferElement(ShaderDataType type) : type(type), size(SizeOf(type)), offset(0) { }

  ShaderDataType type;
  size_t size;
  size_t offset;

  operator ShaderDataType() { return type; }
};

}
