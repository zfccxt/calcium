#pragma once

#include <memory>

#include "backend.hpp"
#include "window.hpp"

namespace cl {

class Instance {
public:
  virtual std::unique_ptr<Window> CreateWindow() const = 0;
};

std::unique_ptr<Instance> CreateInstance(Backend backend = Backend::kVulkan);

}
