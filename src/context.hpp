#pragma once

#include <memory>

#include "backend.hpp"
#include "window.hpp"
#include "window_create_info.hpp"

namespace cl {

class Context {
public:
  virtual ~Context() = default;

  virtual std::unique_ptr<Window> CreateWindow() const = 0;
  virtual std::unique_ptr<Window> CreateWindow(size_t width, size_t height) const = 0;
  virtual std::unique_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) const = 0;
};

std::unique_ptr<Context> CreateContext(Backend backend = Backend::kVulkan);

}