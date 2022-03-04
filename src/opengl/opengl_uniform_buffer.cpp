#include "opengl_uniform_buffer.hpp"

#include "instrumentor.hpp"

namespace cl::opengl {

OpenGLUniformBuffer::OpenGLUniformBuffer(GLuint program_id, size_t binding, size_t size, const std::string& uniform_block_name) : size_(size) {
  CALCIUM_PROFILE_FUNCTION();

  // Index of block where uniform will be uploaded
  GLuint location = glGetUniformBlockIndex(program_id, uniform_block_name.c_str());

  // If location_ == -1, the uniform buffer was probably unused and got optimised away
  if (location != -1) {
    // Link the shader's uniform block to this uniform binding point
    glUniformBlockBinding(program_id, location, binding);
  
    // Create uniform buffer and assign memory for it
    glGenBuffers(1, &memory_);
    glBindBuffer(GL_UNIFORM_BUFFER, memory_);
    glBufferData(GL_UNIFORM_BUFFER, size_, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, binding, memory_, 0, size_);
  }
  else {
    memory_ = -1;
  }
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() {
  CALCIUM_PROFILE_FUNCTION();

  if (memory_ != -1) {
    glDeleteBuffers(1, &memory_);
  }
}

void OpenGLUniformBuffer::Upload(void* data) {
  CALCIUM_PROFILE_FUNCTION();

  if (memory_ != -1) {
    glBindBuffer(GL_UNIFORM_BUFFER, memory_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, size_, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
}


}
