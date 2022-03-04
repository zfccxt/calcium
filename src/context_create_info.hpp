#pragma once

#include <vector>

#include "backend.hpp"
#include "extension_details.hpp"

namespace cl {

struct ContextCreateInfo {
  Backend backend;
  std::vector<ExtensionDetails> extensions;
};

}
