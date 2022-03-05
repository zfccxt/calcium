#pragma once

#include <functional>

namespace cl {

class Context;
class RenderTarget;

typedef std::function<void()>                                     KeyCallback;
typedef std::function<void()>                                     ResizeCallback;
typedef std::function<void(float, float)>                         MouseWheelCallback;
typedef std::function<void()>                                     MouseButtonCallback;
typedef std::function<void(float, float)>                         MouseMoveCallback;
                                                                  
typedef std::function<void(const std::shared_ptr<Context>&)>      ClxCreateCallback;
typedef std::function<void()>                                     ClxDestroyCallback;
typedef std::function<void(const std::shared_ptr<RenderTarget>&)> ClxBindRenderTargetCallback;

}
