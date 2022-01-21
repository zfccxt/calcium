#pragma once

#include "instance.hpp"

namespace cl::Vulkan {

class VulkanInstance : public Instance {
public:
  virtual std::unique_ptr<Window> CreateWindow() const override;
  virtual std::unique_ptr<Window> CreateWindow(size_t width, size_t height) const override;
  virtual std::unique_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) const override;
};

}
