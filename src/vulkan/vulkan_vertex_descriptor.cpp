#include "vulkan_vertex_descriptor.hpp"

#include <vector>

#include "instrumentor.hpp"
#include "shader_data_type.hpp"

namespace cl::Vulkan {

VkVertexInputBindingDescription CreateInputBindingDescription(const BufferLayout& layout) {
  CALCIUM_PROFILE_FUNCTION();

  // First we describe to Vulkan how to pass this data to the vertex shader
  VkVertexInputBindingDescription binding_description { };
  // binding specifies the index of the binding in the array of bindings
  binding_description.binding = 0;
  // stride is the number of bytes from one vertex to the next
  // This is just the size of a vertex since the data is interleaved
  binding_description.stride = layout.GetStride();
  // We tell Vulkan to move to the next data entry after each vertex
  binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  return binding_description;
}

#pragma warning(push)
#pragma warning(disable : 26812)

VkFormat ShaderDataTypeToVulkanFormat(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::kBool:      return VK_FORMAT_R8_SINT;
    case ShaderDataType::kInt:       return VK_FORMAT_R32_SINT;
    case ShaderDataType::kInt2:      return VK_FORMAT_R32G32_SINT;
    case ShaderDataType::kInt3:      return VK_FORMAT_R32G32B32_SINT;
    case ShaderDataType::kInt4:      return VK_FORMAT_R32G32B32A32_SINT;
    case ShaderDataType::kFloat:     return VK_FORMAT_R32_SFLOAT;
    case ShaderDataType::kFloat2:    return VK_FORMAT_R32G32_SFLOAT;
    case ShaderDataType::kFloat3:    return VK_FORMAT_R32G32B32_SFLOAT;
    case ShaderDataType::kFloat4:    return VK_FORMAT_R32G32B32A32_SFLOAT;
    // We don't support using matrices as vertex data inputs
    // A common idiom is to upload 4 float4s instead and construct the matrix in the shader
    // Either that or just use uniforms or push constants instead like a sensible person
    // TODO: Make this explicit to the user of the library
    case ShaderDataType::kMatrix3:   return VK_FORMAT_MAX_ENUM;
    case ShaderDataType::kMatrix4:   return VK_FORMAT_MAX_ENUM;
    default:                         return VK_FORMAT_MAX_ENUM;
  }
}

#pragma warning(pop)

std::vector<VkVertexInputAttributeDescription> CreateInputAttribDescriptions(const BufferLayout& layout) {
  CALCIUM_PROFILE_FUNCTION();

  std::vector<VkVertexInputAttributeDescription> attribute_descriptions(layout.GetNumElements());

  for (size_t i = 0; i < layout.GetNumElements(); ++i) {
    // binding specifies the index of the binding from which the per-vertex data will come
    attribute_descriptions[i].binding = 0;
    // A BufferLayout must be constructed with its elements in the same order as they are specified in the shader
    attribute_descriptions[i].location = i;
    // We must map each Calcium data type to an appropriate Vulkan data format
    attribute_descriptions[i].format = ShaderDataTypeToVulkanFormat(layout[i].type);
    // We have already calculated the offset - that's handy
    attribute_descriptions[i].offset = layout[i].offset;
  }

  return attribute_descriptions;
}

}
