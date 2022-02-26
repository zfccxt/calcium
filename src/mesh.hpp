#pragma once

#include <string>

#include "mesh_data.hpp"

namespace cl {

class Mesh {
public:
  virtual void Draw() = 0;

public:
  static MeshData LoadObj(const std::string& file_path);
};

}
