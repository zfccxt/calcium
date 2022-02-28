#ifdef CALCIUM_BUILD_PROFILE

#include "instrumentor.hpp"

#include <algorithm>
#include <fstream>

namespace cl::Instrumentor {

static std::ofstream output_stream_;
int profile_count_;

void WriteHeader() {
  output_stream_ << "{\"otherData\": {},\"traceEvents\":[";
  output_stream_.flush();
}

void WriteFooter() {
  output_stream_ << "]}";
  output_stream_.flush();
}


void BeginSession(const std::string& file_path) {
  profile_count_ = 0;
  output_stream_.open(file_path);
  WriteHeader();
}

void EndSession() {
  WriteFooter();
  output_stream_.close();
  profile_count_ = 0;
}

void WriteProfile(const char* name, long long start, long long end, uint32_t thread_id) {
  if (profile_count_++ > 0) {
    output_stream_ << ",";
  }

  std::string str_name = name;
  std::replace(str_name.begin(), str_name.end(), '"', '\'');

  output_stream_ << "{";
  output_stream_ << "\"cat\":\"function\",";
  output_stream_ << "\"dur\":" << (end - start) << ',';
  output_stream_ << "\"name\":\"" << str_name << "\",";
  output_stream_ << "\"ph\":\"X\",";
  output_stream_ << "\"pid\":0,";
  output_stream_ << "\"tid\":" << thread_id << ",";
  output_stream_ << "\"ts\":" << start;
  output_stream_ << "}";

  output_stream_.flush();
}

}

#endif