#pragma once

#include <vector>

#include "backend.hpp"
#include "extension_details.hpp"

namespace cl {

struct ContextCreateInfo {
  Backend backend = Backend::kVulkan;
  std::vector<ExtensionDetails> extensions;
};

}
