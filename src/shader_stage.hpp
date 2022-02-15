#pragma once

namespace cl {

enum class ShaderStage : char {
  kComputeShader,  kFragmentShader, 
  kGeometryShader, kMeshShader, 
  kTaskShader,     kTesselationShader, 
  kVertexShader
};

}
