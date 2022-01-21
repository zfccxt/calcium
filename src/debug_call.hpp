#pragma once

#if CALCIUM_BUILD_DEBUG
  #define CALCIUM_DEBUG_CALL(x) x
#else
  #define CALCIUM_DEBUG_CALL(x)
#endif
