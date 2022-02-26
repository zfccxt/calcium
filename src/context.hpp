#pragma once

#include <memory>

#include "backend.hpp"
#include "mesh.hpp"
#include "mesh_create_info.hpp"
#include "mesh_data.hpp"
#include "render_target.hpp"
#include "texture.hpp"
#include "texture_create_info.hpp"
#include "shader.hpp"
#include "shader_create_info.hpp"
#include "window.hpp"
#include "window_create_info.hpp"

namespace cl {

class Context {
public:
  virtual ~Context() = default;

  std::shared_ptr<Window> CreateWindow();
  std::shared_ptr<Window> CreateWindow(size_t width, size_t height);
  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& window_info) = 0;

  std::shared_ptr<Shader> CreateShader(const std::string& vert_path, const std::string& frag_path);
  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& shader_info) = 0;

  virtual std::shared_ptr<Mesh> CreateMesh(const MeshCreateInfo& mesh_info) = 0;
  std::shared_ptr<Mesh> CreateMesh(const MeshData& mesh_data);
  std::shared_ptr<Mesh> CreateMesh(const std::string& file_path);

  std::shared_ptr<Texture> CreateTexture(const std::string& file_path);
  virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& texture_info) = 0;

  virtual void BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) = 0;
  virtual void BeginRenderPass(const std::shared_ptr<Shader>& shader) = 0;
  virtual void EndRenderPass() = 0;
};

std::shared_ptr<Context> CreateContext(Backend backend = Backend::kVulkan);

}
