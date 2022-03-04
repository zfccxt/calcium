#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>

#include "opengl_uniform_buffer.hpp"
#include "shader.hpp"
#include "shader_create_info.hpp"

namespace cl::opengl {

class OpenGLShader : public Shader {
public:
  OpenGLShader(const ShaderCreateInfo& shader_info);
  virtual ~OpenGLShader();

  virtual void UploadUniform(int binding, void* data) override;
  virtual void BindTexture(int binding, const std::shared_ptr<Texture>& texture) override;

public:
  void BindAllTextureSamplers(const std::shared_ptr<Texture>& texture);

public:
  void Bind();

private:
  GLuint program_id_;

  // Map of uniform binding -> uniform buffer so that we can choose a buffer to upload data to by its binding
  std::unordered_map<size_t, std::unique_ptr<OpenGLUniformBuffer>> uniforms_;
  
  // Map of sampler binding -> GL texture slot so that we can choose a texture slot by its binding
  std::unordered_map<size_t, size_t> samplers_;
};

}
