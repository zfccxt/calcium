#include "vulkan_shader_module.hpp"

#include "instrumentor.hpp"
#include "vulkan_check.hpp"

namespace cl::vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

VkShaderModule CreateShaderModule(VulkanContextData* context, const SpvCode& code) {
  CALCIUM_PROFILE_FUNCTION();

  VkShaderModuleCreateInfo create_info { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
  create_info.codeSize = code.size() * sizeof(uint32_t);
  create_info.pCode = code.data();

  VkShaderModule shader_module;
  VK_CHECK(vkCreateShaderModule(context->device, &create_info, context->allocator, &shader_module));
  return shader_module;
}

VkShaderStageFlagBits FindVulkanShaderStage(ShaderStage stage) {
  switch (stage) {
    case ShaderStage::kComputeShader:     return VK_SHADER_STAGE_COMPUTE_BIT;
    case ShaderStage::kFragmentShader:    return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderStage::kGeometryShader:    return VK_SHADER_STAGE_GEOMETRY_BIT;
    case ShaderStage::kMeshShader:        return VK_SHADER_STAGE_MESH_BIT_NV;
    case ShaderStage::kTaskShader:        return VK_SHADER_STAGE_TASK_BIT_NV;
    case ShaderStage::kTesselationShader: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case ShaderStage::kVertexShader:      return VK_SHADER_STAGE_VERTEX_BIT;
    default:                              return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
  }
}

#pragma warning(pop)

}
