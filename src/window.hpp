#pragma once

namespace cl {

class Window {
public:
  virtual ~Window() = default;

  virtual bool IsOpen() const = 0;
};

void PollWindowEvents();

}
