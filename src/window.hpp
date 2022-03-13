#pragma once

#include "colour.hpp"
#include "key_code.hpp"
#include "render_target.hpp"
#include "callbacks.hpp"

namespace cl {

class Window : public RenderTarget {
public:
  virtual ~Window() = default;

  virtual float GetAspectRatio() const = 0;
  virtual bool IsOpen() const = 0;
  virtual void Close() = 0;
  virtual void Center(bool center_horizontal=true, bool center_vertical=true) = 0;

  virtual void SetClearColour(const Colour& colour) = 0;

  virtual void SetDepthTestEnable(bool enable) = 0;
  virtual bool IsDepthTestEnabled() const = 0;

  virtual void PollEvents() = 0;

  virtual bool IsCursorLocked() = 0;

  virtual void LockCursor() = 0;
  virtual void UnlockCursor() = 0;
  virtual void ToggleCursorLock() = 0;

  virtual float CursorX() = 0;
  virtual float CursorY() = 0;

  virtual bool IsKeyDown(KeyCode key) = 0;

  virtual void SetResizeCallback(ResizeCallback) = 0;
  virtual void RemoveResizeCallback() = 0;

  virtual void SetKeyPressCallback(KeyCode, KeyCallback) = 0;
  virtual void RemoveKeyPressCallback(KeyCode) = 0;
  virtual void SetKeyReleaseCallback(KeyCode, KeyCallback) = 0;
  virtual void RemoveKeyReleaseCallback(KeyCode) = 0;

  virtual void SetMouseButtonPressCallback(MouseButton, MouseButtonCallback) = 0;
  virtual void RemoveMouseButtonPressCallback(MouseButton) = 0;
  virtual void SetMouseButtonReleaseCallback(MouseButton, MouseButtonCallback) = 0;
  virtual void RemoveMouseButtonReleaseCallback(MouseButton) = 0;
  virtual void SetMouseWheelCallback(MouseWheelCallback) = 0;
  virtual void RemoveMouseWheelCallback() = 0;
  virtual void SetMouseMoveCallback(MouseMoveCallback) = 0;
  virtual void RemoveMouseMoveCallback() = 0;

  virtual bool IsControllerDetected() = 0;
  virtual bool IsControllerButtonDown(ControllerButton) = 0;

  virtual float ControllerLeftStickX() = 0;
  virtual float ControllerLeftStickY() = 0;
  virtual float ControllerRightStickX() = 0;
  virtual float ControllerRightStickY() = 0;
};

}
