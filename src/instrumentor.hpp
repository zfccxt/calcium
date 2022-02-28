#pragma once

#ifdef CALCIUM_BUILD_PROFILE

#include <cstdint>
#include <string>

#include "instrumentation_timer.hpp"

namespace cl::Instrumentor {

void BeginSession(const std::string& file_path);
void EndSession();
void WriteProfile(const char* name, long long start, long long end, uint32_t thread_id);

}

	// Make sure we are using the best funcsig available on the current compiler
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define CALCIUM_FUNCSIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define CALCIUM_FUNCSIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define CALCIUM_FUNCSIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define CALCIUM_FUNCSIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define CALCIUM_FUNCSIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define CALCIUM_FUNCSIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define CALCIUM_FUNCSIG __func__
	#else
		#define CALCIUM_FUNCSIG "Unknown CALCIUM_FUNCSIG"
	#endif

  #define CALCIUM_PROFILE_BEGIN(file_path)       ::cl::Instrumentor::BeginSession(file_path)
  #define CALCIUM_PROFILE_END()                  ::cl::Instrumentor::EndSession()

	#define CALCIUM_PROFILE_SCOPE_LINE(name, line) const ::cl::Timer timer##line(name)
  #define CALCIUM_PROFILE_SCOPE(name)            CALCIUM_PROFILE_SCOPE_LINE(name, __LINE__)
  #define CALCIUM_PROFILE_FUNCTION()             CALCIUM_PROFILE_SCOPE(CALCIUM_FUNCSIG)

#else
  #define CALCIUM_PROFILE_BEGIN(file_path)
  #define CALCIUM_PROFILE_END()
  #define CALCIUM_PROFILE_FUNCTION()
  #define CALCIUM_PROFILE_SCOPE(name)
#endif