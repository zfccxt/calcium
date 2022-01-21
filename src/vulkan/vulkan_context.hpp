#pragma once

#include "context.hpp"
#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanContext : public Context {
public:
  VulkanContext();
  virtual ~VulkanContext();

  virtual std::unique_ptr<Window> CreateWindow() const override;
  virtual std::unique_ptr<Window> CreateWindow(size_t width, size_t height) const override;
  virtual std::unique_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) const override;

private:
  VulkanContextData context_data_;
};

}
