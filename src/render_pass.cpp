#include "render_pass.hpp"

namespace cl {

std::shared_ptr<Shader> RenderPass::CreateShader(const std::string& vert_path, const std::string& frag_path) {
  ShaderCreateInfo shader_info;
  shader_info.vert_path = vert_path;
  shader_info.frag_path = frag_path;
  return CreateShader(shader_info);
}

}
