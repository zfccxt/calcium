#pragma once

#include <memory>

#include "backend.hpp"
#include "mesh.hpp"
#include "mesh_create_info.hpp"
#include "render_target.hpp"
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
  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) = 0;

  std::shared_ptr<Shader> CreateShader(const std::string& vert_path, const std::string& frag_path);
  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& create_info) = 0;

  virtual std::shared_ptr<Mesh> CreateMesh(const MeshCreateInfo& file_path) = 0;

  virtual void BindRendertarget(const std::shared_ptr<RenderTarget>& render_target) = 0;
};

std::unique_ptr<Context> CreateContext(Backend backend = Backend::kVulkan);

}
