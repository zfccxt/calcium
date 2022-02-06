#pragma once

#include <string>

namespace cl {

struct ShaderCreateInfo {
  std::string vert_path;
  std::string frag_path;
  std::string geom_path;
  std::string mesh_path;
  std::string task_path;
  std::string tess_path;
  std::string comp_path;
};

}
