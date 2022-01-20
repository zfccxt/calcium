#pragma once

#include "instance.hpp"

namespace cl::Vulkan {

class VulkanInstance : public Instance {
public:
  virtual std::unique_ptr<Window> CreateWindow() const override;
};

}
