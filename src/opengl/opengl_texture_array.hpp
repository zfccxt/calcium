#pragma once

#include <glad/glad.h>

#include "texture_array.hpp"
#include "texture_array_create_info.hpp"

namespace cl::opengl {

class OpenGLTextureArray : public TextureArray {
public:
  OpenGLTextureArray(const TextureArrayCreateInfo& texture_array_info);
  ~OpenGLTextureArray();

  virtual size_t GetWidth() const override;
  virtual size_t GetHeight() const override;

private:
  GLuint texture_id_;

  size_t width_, height_;
};

}
