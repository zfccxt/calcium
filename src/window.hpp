#pragma once

#include "colour.hpp"
#include "render_target.hpp"

namespace cl {

class Window : public RenderTarget {
public:
  virtual ~Window() = default;

  virtual float GetAspectRatio() const = 0;
  virtual bool IsOpen() const = 0;
  virtual void Center(bool center_horizontal=true, bool center_vertical=true) = 0;

  virtual void Clear() = 0;
  virtual void SwapBuffers() = 0;

  virtual void SetClearColour(const Colour& colour) = 0;

  virtual void PollEvents() = 0;
};

}
