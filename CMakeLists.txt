cmake_minimum_required(VERSION 3.21)
project(calcium)

set(CMAKE_CONFIGURATION_TYPES Verbose Debug Profile Release Distribution)

set(CMAKE_CXX_FLAGS_VERBOSE "${CMAKE_CXX_FLAGS_DEBUG} -DCALCIUM_BUILD_DEBUG=1 -DCALCIUM_BUILD_VERBOSE=1")
set(CMAKE_EXE_LINKER_FLAGS_VERBOSE "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -DCALCIUM_BUILD_PROFILE=1 -DCALCIUM_BUILD_RELEASE=1")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_RELEASE} -DCALCIUM_BUILD_RELEASE=1 -DCALCIUM_BUILD_DISTRIBUTION=1")
set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCALCIUM_BUILD_DEBUG=1")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DCALCIUM_BUILD_RELEASE=1")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

set(CALCIUM_FILES src/calcium.hpp src/backend.hpp src/context.hpp src/context.cpp src/uid.hpp src/uid.cpp src/window.hpp src/window.cpp src/window_create_info.hpp src/glfw_window.hpp src/glfw_window.cpp src/glfw_utils.hpp src/glfw_utils.cpp src/debug_call.hpp)
source_group("src" FILES ${CALCIUM_FILES})

set(OPENGL_FILES src/opengl/opengl_context.hpp src/opengl/opengl_context.cpp src/opengl/opengl_window.hpp src/opengl/opengl_window.cpp)
source_group("src/opengl" FILES ${OPENGL_FILES})

find_package(Vulkan)
if (Vulkan_FOUND)
  set(VULKAN_FILES src/vulkan/vulkan_context.hpp src/vulkan/vulkan_context.cpp src/vulkan/vulkan_window.hpp src/vulkan/vulkan_window.cpp src/vulkan/vulkan_allocator.hpp src/vulkan/vulkan_allocator.cpp src/vulkan/vulkan_context_data.hpp src/vulkan/vulkan_instance.hpp src/vulkan/vulkan_instance.cpp src/vulkan/vulkan_check.hpp src/vulkan/vulkan_check.cpp src/vulkan/vulkan_debug_messenger.hpp src/vulkan/vulkan_debug_messenger.cpp)
  source_group("src/vulkan" FILES ${VULKAN_FILES})
endif()

add_library(${PROJECT_NAME} ${CALCIUM_FILES} ${OPENGL_FILES} ${VULKAN_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC src)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
if(PROJECT_IS_TOP_LEVEL)
  if(APPLE)
    set_target_properties(calcium_test PROPERTIES XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT calcium_test)
    set_property(TARGET calcium_test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
endif()

add_subdirectory(depend/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
add_subdirectory(depend/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

if(APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC CALCIUM_PLATFORM_MACOS=1)
endif()
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PUBLIC CALCIUM_PLATFORM_WINDOWS=1)
endif()

if(Vulkan_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARY})
  target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})
  target_compile_definitions(${PROJECT_NAME} PUBLIC CALCIUM_VULKAN_SDK_FOUND=1)
endif()
