#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>

// TODO: uniform buffers should be able to be shared between shaders

namespace cl::opengl {

class OpenGLUniformBuffer {
public:
  OpenGLUniformBuffer(GLuint program_id, size_t binding, size_t size, const std::string& uniform_block_name);
  ~OpenGLUniformBuffer();

  void Upload(void* data);

private:
  GLuint memory_;
  size_t size_;
};

}
