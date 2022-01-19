#pragma once

#include <memory>
#include "backend.hpp"

namespace cl {

class Instance {

};

std::unique_ptr<Instance> CreateInstance(Backend backend = Backend::kVulkan);

}
