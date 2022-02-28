#pragma once

#include <functional>

namespace cl {

typedef std::function<void()> KeyCallback;
typedef std::function<void()> ResizeCallback;
typedef std::function<void(float, float)> MouseWheelCallback;
typedef std::function<void()> MouseButtonCallback;
typedef std::function<void(float, float)> MouseMoveCallback;

}
