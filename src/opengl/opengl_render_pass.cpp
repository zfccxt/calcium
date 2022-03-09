#include "opengl_render_pass.hpp"

#include <assert.h>

#include "instrumentor.hpp"
#include "opengl/opengl_shader.hpp"
#include "opengl/opengl_context.hpp"

namespace cl::opengl {

OpenGLRenderPass::OpenGLRenderPass(const RenderPassCreateInfo& render_pass_info)
  : load_op_flags_(render_pass_info.load_op_flags),
    enable_depth_test_(render_pass_info.enable_depth_test) {

  render_target_ = (OpenGLWindow*)render_pass_info.render_target.get();
  assert(render_target_);

  gl_clear_bits_ = GL_COLOR_BUFFER_BIT | (enable_depth_test_ ? GL_DEPTH_BUFFER_BIT : 0);
}

void OpenGLRenderPass::Begin() {
  render_target_->MakeContextCurrent();

  if (enable_depth_test_) {
    glEnable(GL_DEPTH_TEST);
  }
  else {
    glDisable(GL_DEPTH_TEST);
  }

  if (load_op_flags_ & RenderPassLoadOp_Clear) {
    render_target_->Clear(gl_clear_bits_);
  }
}

void OpenGLRenderPass::End() { /* Intentionally empty */ }

std::shared_ptr<Shader> OpenGLRenderPass::CreateShader(const ShaderCreateInfo& shader_info) {
  CALCIUM_PROFILE_FUNCTION();

  auto shader = std::make_shared<OpenGLShader>(shader_info);
  shader->Bind();
  shader->BindAllTextureSamplers(render_target_->GetContext()->GetBlankTexture());
  return shader;
}

}
