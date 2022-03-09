#pragma once

#include <memory>

#include "shader.hpp"
#include "shader_create_info.hpp"

namespace cl {

class RenderPass {
public:
  virtual void Begin() = 0;
  virtual void End() = 0;

  std::shared_ptr<Shader> CreateShader(const std::string& vert_path, const std::string& frag_path);
  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& shader_info) = 0;
};

}
