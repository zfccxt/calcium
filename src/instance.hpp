#pragma once

#include <memory>

#include "backend.hpp"
#include "window.hpp"
#include "window_create_info.hpp"

namespace cl {

class Instance {
public:
  virtual std::unique_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) const = 0;
};

std::unique_ptr<Instance> CreateInstance(Backend backend = Backend::kVulkan);

}
