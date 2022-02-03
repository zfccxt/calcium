#include "shader.hpp"

#include <assert.h>
#include <fstream>

namespace cl {

std::vector<uint32_t> Shader::ReadSPV(const char* file_path) {
  std::ifstream file(file_path, std::ios::ate | std::ios::binary);
  assert(file.is_open());
  size_t file_size = (size_t)file.tellg();
  std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));
  file.seekg(0);
  file.read((char*)buffer.data(), file_size);
  file.close();
  return buffer;
}

}
