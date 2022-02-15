#include "shader_reflection_details.hpp"

// spirv-cross is also included with the Vulkan SDK. Make sure not to #include <spirv-cross/spirv_xxx> as this will
// cause mismatched header and cpp files and you will end up with linker errors
#include <spirv_glsl.hpp>

namespace cl {

ShaderDataType FindVectorType(uint32_t size) {
  switch (size) {
    case 1:  return ShaderDataType::kFloat;
    case 2:  return ShaderDataType::kFloat2;
    case 3:  return ShaderDataType::kFloat3;
    case 4:  return ShaderDataType::kFloat4;
    default: return ShaderDataType::kUndefined;
  }
}

void ShaderReflectionDetails::Reflect(const ShaderCodeMap& shader_code) {
  // TODO: find samplers

  std::unordered_map<uint32_t, ShaderDataType> vertex_input_map;
  std::unordered_map<uint32_t, UniformData> uniform_map;

  for (const auto& shader : shader_code) {
    const ShaderStage current_stage = shader.first;

    spirv_cross::CompilerGLSL* glsl = new spirv_cross::CompilerGLSL(shader.second);
	  spirv_cross::ShaderResources* resources = new spirv_cross::ShaderResources(glsl->get_shader_resources());

    // Find vertex input vectors
		for (auto& stage_input : resources->stage_inputs) {
      uint32_t location = glsl->get_decoration(stage_input.id, spv::DecorationLocation);
      spirv_cross::SPIRType type = glsl->get_type(stage_input.type_id);
      ShaderDataType data_type = FindVectorType(type.vecsize);
      if (data_type != ShaderDataType::kUndefined) {
        vertex_input_map.emplace(location, data_type);
      }
    }

    // Find uniforms
    for (auto& uniform : resources->uniform_buffers) {
      UniformData uniform_data;
      uniform_data.binding =  glsl->get_decoration(uniform.id, spv::DecorationBinding);
      uniform_data.stage = current_stage;
      uniform_data.size = glsl->get_declared_struct_size(glsl->get_type(uniform.base_type_id));
      uniform_data.name = glsl->get_name(uniform.id);
      uniform_map.emplace(uniform_data.binding, uniform_data);
    }

    delete resources;
    delete glsl;
  }

  // Populate reflection details
  // vertex input layout
  std::vector<BufferElement> buffer_elements;
  for (uint32_t i = 0; i < vertex_input_map.size(); ++i) {
    buffer_elements.push_back(vertex_input_map.at(i));
  }
  vertex_input_layout = buffer_elements;

  // uniforms
  for (uint32_t i = 0; i < uniform_map.size(); ++i) {
    uniforms.push_back(uniform_map.at(i));
  }
}

}
