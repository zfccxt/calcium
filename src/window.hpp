#pragma once

namespace cl {

class Window {
public:
  virtual ~Window() = default;

  virtual bool IsOpen() const = 0;
  virtual void Center(bool center_horizontal=true, bool center_vertical=true) = 0;
};

void PollWindowEvents();

}
