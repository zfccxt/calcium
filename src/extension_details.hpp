#pragma once

#include "callbacks.hpp"

namespace cl {

struct ExtensionDetails {
  ClxCreateCallback           on_create;
  ClxDestroyCallback          on_destroy;
  ClxBindRenderTargetCallback on_bind_render_target;
};

}
