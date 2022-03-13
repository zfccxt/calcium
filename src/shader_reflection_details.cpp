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
  std::unordered_map<size_t, ShaderDataType> vertex_input_map;

  for (const auto& shader : shader_code) {
    const ShaderStage current_stage = shader.first;

    // TODO: This works but is not really best practice or how the library is intended to be used
    spirv_cross::CompilerGLSL* glsl = new spirv_cross::CompilerGLSL(shader.second);
	  spirv_cross::ShaderResources* resources = new spirv_cross::ShaderResources(glsl->get_shader_resources());

    // Find vertex input vectors
    if (current_stage == ShaderStage::kVertexShader) {
		  for (auto& stage_input : resources->stage_inputs) {
        uint32_t location = glsl->get_decoration(stage_input.id, spv::DecorationLocation);
        spirv_cross::SPIRType type = glsl->get_type(stage_input.type_id);
        ShaderDataType data_type = FindVectorType(type.vecsize);
        if (data_type != ShaderDataType::kUndefined) {
          vertex_input_map.emplace(location, data_type);
        }
      }
    }

    // Find uniforms
    for (auto& uniform : resources->uniform_buffers) {
      UniformData uniform_data;
      uniform_data.binding = glsl->get_decoration(uniform.id, spv::DecorationBinding);
      uniform_data.stage = current_stage;
      uniform_data.size = glsl->get_declared_struct_size(glsl->get_type(uniform.base_type_id));
      uniform_data.name = glsl->get_name(uniform.id);
      uniform_data.uniform_block_name = glsl->get_name(uniform.base_type_id);
      uniforms.emplace(uniform_data.binding, uniform_data);
    }

    // Find texture samplers
    for (auto& texture : resources->sampled_images) {
      auto& type = glsl->get_type(texture.type_id);

      if (!type.image.arrayed) {
        // Single texture samplers (sampler2D)
        TextureData texture_data;
        texture_data.binding = glsl->get_decoration(texture.id, spv::DecorationBinding);
        texture_data.stage = current_stage;
        texture_data.name = glsl->get_name(texture.id);
        textures.emplace(texture_data.binding, texture_data);
      }
      else {
        // Texture sampler arrays (sampler2DArray)
        TextureArrayData texture_array_data;
        texture_array_data.binding = glsl->get_decoration(texture.id, spv::DecorationBinding);
        texture_array_data.stage = current_stage;
        texture_array_data.name = glsl->get_name(texture.id);
        texture_arrays.emplace(texture_array_data.binding, texture_array_data);
      }
    }

    delete resources;
    delete glsl;
  }

  // Populate vertex input BufferLayout
  std::vector<BufferElement> buffer_elements;
  for (uint32_t i = 0; i < vertex_input_map.size(); ++i) {
    buffer_elements.push_back(vertex_input_map.at(i));
  }
  vertex_input_layout = buffer_elements;
}

}
