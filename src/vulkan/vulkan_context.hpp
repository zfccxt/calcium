#pragma once

#include "context.hpp"
#include "vulkan/vulkan_context_data.hpp"

namespace cl::Vulkan {

class VulkanContext : public Context {
public:
  VulkanContext();
  virtual ~VulkanContext();

  virtual std::unique_ptr<Window> CreateWindow() override;
  virtual std::unique_ptr<Window> CreateWindow(size_t width, size_t height) override;
  virtual std::unique_ptr<Window> CreateWindow(const WindowCreateInfo& create_info) override;

  virtual std::unique_ptr<Shader> CreateShader(const std::string& vert_path, const std::string& frag_path) override;
  virtual std::unique_ptr<Shader> CreateShader(const ShaderCreateInfo& create_info) override;

private:
  VulkanContextData context_data_;
};

}
