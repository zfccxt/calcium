#pragma once

namespace cl {

enum class ShaderType : char {
  kComputeShader,  kFragmentShader, 
  kGeometryShader, kMeshShader, 
  kTaskShader,     kTesselationShader, 
  kVertexShader
};

}
