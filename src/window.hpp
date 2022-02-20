#pragma once

#include "colour.hpp"
#include "key_code.hpp"
#include "render_target.hpp"
#include "key_callback.hpp"

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

  virtual bool IsCursorLocked() = 0;

  virtual void LockCursor() = 0;
  virtual void UnlockCursor() = 0;
  virtual void ToggleCursorLock() = 0;

  virtual float CursorDeltaX() = 0;
  virtual float CursorDeltaY() = 0;
  virtual float CursorX() = 0;
  virtual float CursorY() = 0;

  virtual bool IsKeyDown(KeyCode key) = 0;

  virtual void SetKeyPressCallback(KeyCode, KeyCallback) = 0;
  virtual void RemoveKeyPressCallback(KeyCode) = 0;
  virtual void SetKeyReleaseCallback(KeyCode, KeyCallback) = 0;
  virtual void RemoveKeyReleaseCallback(KeyCode) = 0;
};

}
