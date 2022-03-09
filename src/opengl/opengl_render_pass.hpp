#pragma once

#include "render_pass.hpp"
#include "render_pass_create_info.hpp"
#include "opengl/opengl_window.hpp"

namespace cl::opengl {

class OpenGLRenderPass : public RenderPass {
public:
  OpenGLRenderPass(const RenderPassCreateInfo& render_pass_info);
 
  virtual void Begin() override;
  virtual void End() override;

  virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& shader_info) override;

private:
  RenderPassLoadOpFlags load_op_flags_;
  bool enable_depth_test_;
  // Should probably be a weak_ptr
  OpenGLWindow* render_target_;

  unsigned int gl_clear_bits_;
};

}
