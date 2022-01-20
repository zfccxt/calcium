#pragma once

#include <cstdint>

namespace cl {

class Uid {
public:
  Uid(uint64_t value);

  static Uid Generate();

  inline bool operator==(const Uid& other) { return uid_ == other.uid_; }
  inline bool operator!=(const Uid& other) { return uid_ != other.uid_; }

public:
  static Uid kNull;

private:
  uint64_t uid_;

private:
  static uint64_t current_uid_;
};

}
