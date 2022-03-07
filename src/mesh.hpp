#pragma once

#include <string>

#include "mesh_create_info.hpp"

namespace cl {

class Mesh {
public:
  virtual void Draw() = 0;

public:
  static MeshCreateInfo LoadObj(const std::string& file_path);
};

}
