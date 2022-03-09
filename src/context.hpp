#pragma once

#include <memory>
#include <vector>

#include "backend.hpp"
#include "context_create_info.hpp"
#include "mesh.hpp"
#include "mesh_create_info.hpp"
#include "render_pass.hpp"
#include "render_pass_create_info.hpp"
#include "render_target.hpp"
#include "texture.hpp"
#include "texture_create_info.hpp"
#include "window.hpp"
#include "window_create_info.hpp"

namespace cl {

class Context {
public:
  virtual ~Context();

  std::shared_ptr<Window> CreateWindow();
  std::shared_ptr<Window> CreateWindow(size_t width, size_t height);
  virtual std::shared_ptr<Window> CreateWindow(const WindowCreateInfo& window_info) = 0;

  virtual std::shared_ptr<Mesh> CreateMesh(const MeshCreateInfo& mesh_info) = 0;
  std::shared_ptr<Mesh> CreateMesh(const std::string& file_path);

  virtual std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassCreateInfo&) = 0;
  std::shared_ptr<RenderPass> CreateRenderPass(const std::shared_ptr<RenderTarget>&);

  std::shared_ptr<Texture> CreateTexture(const std::string& file_path);
  virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& texture_info) = 0;

  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;

  inline Backend GetBackend() const { return backend_; }  

protected:
  void ClxOnCreate(const std::shared_ptr<Context>& context);
  void ClxOnDestroy();

public:
  static std::shared_ptr<Context> CreateContext(const ContextCreateInfo& context_info);
  static std::shared_ptr<Context> CreateContext(Backend backend = Backend::kVulkan);

private:
  Backend backend_ = Backend::kVulkan;
  std::vector<ExtensionDetails> extensions_;
};


}
