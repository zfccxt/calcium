#pragma once

#ifdef CALCIUM_BUILD_PROFILE

#include <chrono>

namespace cl {

class Timer {
public:
  Timer(const char* name);
  ~Timer();
  
  void Stop();

private:
  const char* name_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_;
  bool stopped_ = false;
};

}

#endif