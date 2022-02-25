#pragma once

#include <glad/glad.h>

#include "texture.hpp"
#include "texture_create_info.hpp"

namespace cl::OpenGL {

class OpenGLTexture : public Texture {
public:
  OpenGLTexture(const TextureCreateInfo& texture_info);
  OpenGLTexture(const BlankTextureCreateInfo& texture_info);
  ~OpenGLTexture();

  virtual void Bind() override;

private:
  GLuint texture_id_;
};

}
