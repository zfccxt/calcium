#pragma once

#include <glad/glad.h>

#include "texture.hpp"
#include "texture_create_info.hpp"

namespace cl::opengl {

class OpenGLTexture : public Texture {
public:
  OpenGLTexture(const TextureCreateInfo& texture_info);
  OpenGLTexture(const BlankTextureCreateInfo& texture_info);
  ~OpenGLTexture();

  virtual size_t GetWidth() const override;
  virtual size_t GetHeight() const override;

public:
  void Bind() const;

public:
  static GLenum TextureFilterToGLMinFilter(TextureFilter filter);
  static GLenum TextureFilterToGLMagFilter(TextureFilter filter);
  static GLenum TextureWrapModeToGLEnum(TextureWrap wrap_mode);

private:
  GLuint texture_id_;

  size_t width_, height_;
};

}
