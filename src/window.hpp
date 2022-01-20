#pragma once

namespace cl {

class Window {
public:
  virtual ~Window() = default;

  virtual bool IsOpen() const = 0;
  virtual void PollEvents() const = 0;
};

}
