#include "shader_reflection_details.hpp"

// spirv-cross is also included with the Vulkan SDK. Make sure not to #include <spirv-cross/spirv_xxx> as this will
// cause mismatched header and cpp files and you will end up with linker errors
#include <spirv_glsl.hpp>

#include "shader_data_type.hpp"

namespace cl {

ShaderDataType FindType(uint32_t size) {
  switch (size) {
    case 1:  return ShaderDataType::kFloat;
    case 2:  return ShaderDataType::kFloat2;
    case 3:  return ShaderDataType::kFloat3;
    case 4:  return ShaderDataType::kFloat4;
    default: return ShaderDataType::kUndefined;
  }
}

void ShaderReflectionDetails::Reflect(const ShaderCodeMap& shader_code) {
  // TODO: find uniform buffers
  // TODO: find samplers

  std::unordered_map<uint32_t, ShaderDataType> vertex_input_map;

  for (const auto& shader : shader_code) {
    spirv_cross::CompilerGLSL* glsl = new spirv_cross::CompilerGLSL(shader.second);
	  spirv_cross::ShaderResources* resources = new spirv_cross::ShaderResources(glsl->get_shader_resources());

    // Find vertex input vectors
		for (auto& stage_input : resources->stage_inputs) {
      uint32_t location = glsl->get_decoration(stage_input.id, spv::DecorationLocation);
      spirv_cross::SPIRType type = glsl->get_type(stage_input.type_id);
      ShaderDataType data_type = FindType(type.vecsize);
      if (data_type != ShaderDataType::kUndefined) {
        vertex_input_map.emplace(location, data_type);
      }
    }

    delete resources;
    delete glsl;
  }

  std::vector<BufferElement> buffer_elements;
  for (uint32_t i = 0; i < vertex_input_map.size(); ++i) {
    buffer_elements.push_back(vertex_input_map.at(i));
  }
  vertex_input_layout = buffer_elements;
}

}
