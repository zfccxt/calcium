cmake_minimum_required(VERSION 3.21)
project(calcium_test)

set(SOURCE_FILES src/main.cpp)
source_group("src" FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} PRIVATE calcium)

target_include_directories(${PROJECT_NAME} PRIVATE src)
