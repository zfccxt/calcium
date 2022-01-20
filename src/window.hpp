#pragma once

namespace cl {

class Window {
public:
  virtual bool IsOpen() const = 0;
  virtual void PollEvents() const = 0;
};

}
