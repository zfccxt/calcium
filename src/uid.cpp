#include "uid.hpp"

#include <cstddef>

namespace cl {

Uid Uid::kNull = -1;
size_t current_uid_ = 0;

Uid::Uid(uint64_t value) : uid_(value) { }

Uid Uid::Generate() {
  Uid uid = current_uid_;
  ++current_uid_;
  return uid;
}

}
