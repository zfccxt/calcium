#include "shader_reflection_details.hpp"

// spirv-cross is also included with the Vulkan SDK. Make sure not to #include <spirv-cross/spirv_xxx> as this will
// cause mismatched header and cpp files and you will end up with linker errors
#include <spirv_glsl.hpp>

namespace cl {

ShaderReflectionDetails::ShaderReflectionDetails(const ShaderCodeMap& shader_code) {
  // TODO: find vertex input
  // TODO: find uniform buffers
  // TODO: find samplers
}

}
