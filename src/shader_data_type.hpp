#pragma once

namespace cl {

enum class ShaderDataType : char {
  kBool,
  kInt,     kInt2,   kInt3,   kInt4,
  kFloat,   kFloat2, kFloat3, kFloat4,
  kMatrix3, kMatrix4,
  kUndefined
};

}
