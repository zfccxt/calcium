#pragma once

#include <cstdint>
#include <vector>

namespace cl {

class Shader {
public:
  virtual ~Shader() = default;

  virtual void Bind() = 0;
  
  static std::vector<uint32_t> ReadSPV(const char* file_path);
};

}
