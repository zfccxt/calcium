#pragma once

#include <memory>
#include <string>

#include "backend.hpp"
#include "shader.hpp"
#include "shader_create_info.hpp"
#include "window.hpp"
#include "window_create_info.hpp"

namespace cl {

class Context {
public:
  virtual ~Context() = default;

  virtual std::unique_ptr<Window> CreateWindow() = 0;
  virtual std::unique_ptr<Window> CreateWindow(size_t width, size_t height) = 0;
  virtual std::unique_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) = 0;

  virtual std::unique_ptr<Shader> CreateShader(const std::string& vert_path, const std::string& frag_path) = 0;
  virtual std::unique_ptr<Shader> CreateShader(const ShaderCreateInfo& create_info) = 0;
};

std::unique_ptr<Context> CreateContext(Backend backend = Backend::kVulkan);

}
