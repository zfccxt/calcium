#pragma once

#include "texture.hpp"
#include "texture_create_info.hpp"

namespace cl::OpenGL {

class OpenGLTexture : public Texture {
public:
  OpenGLTexture(const TextureCreateInfo& texture_info);

  virtual void Bind() override;
};

}