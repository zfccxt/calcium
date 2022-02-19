#pragma once

#include "texture.hpp"
#include "texture_create_info.hpp"

namespace cl::Vulkan {

class VulkanTexture : public Texture {
public:
  VulkanTexture(const TextureCreateInfo& texture_info);

  virtual void Bind() override;
};

}