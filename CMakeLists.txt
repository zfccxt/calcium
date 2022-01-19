cmake_minimum_required(VERSION 3.21)
project(calcium)

set(CALCIUM_FILES src/calcium/calcium.hpp)
source_group("calcium" FILES ${CALCIUM_FILES})

add_library(${PROJECT_NAME} ${CALCIUM_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC src)

add_subdirectory(test)
if (PROJECT_IS_TOP_LEVEL)
  if (APPLE)
    set_target_properties(calcium_test PROPERTIES XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT calcium_test)
    set_property(TARGET calcium_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
endif()

add_subdirectory(depend/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
