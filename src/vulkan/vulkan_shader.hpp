#pragma once

#include "shader.hpp"
#include "shader_create_info.hpp"

namespace cl::Vulkan {

class VulkanShader : public Shader {
public:
  VulkanShader(const ShaderCreateInfo& shader_info);

  virtual void Bind() override;
};

}
