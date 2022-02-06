#include "shader_data_type.hpp"

namespace cl {

size_t SizeOf(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::kBool:    return 4;
  case ShaderDataType::kInt:     return 4;
  case ShaderDataType::kInt2:    return 4 * 2;
  case ShaderDataType::kInt3:    return 4 * 3;
  case ShaderDataType::kInt4:    return 4 * 4;
  case ShaderDataType::kFloat:   return 4;
  case ShaderDataType::kFloat2:  return 4 * 2;
  case ShaderDataType::kFloat3:  return 4 * 3;
  case ShaderDataType::kFloat4:  return 4 * 4;
  case ShaderDataType::kMatrix3: return 4 * 3 * 3;
  case ShaderDataType::kMatrix4: return 4 * 4 * 4;
  }
}

}
