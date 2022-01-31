#pragma once

namespace cl {

class Shader {
public:
  virtual ~Shader() = default;

  virtual void Bind() = 0;
};

}
