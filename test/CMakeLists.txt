cmake_minimum_required(VERSION 3.21)
project(calcium_test)

set(SOURCE_FILES src/main.cpp)
source_group("src" FILES ${SOURCE_FILES})

set(SHADER_FILES src/shaders/shader.vert.glsl src/shaders/shader.frag.glsl)
source_group("src/shaders" FILES ${SHADER_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SHADER_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} PRIVATE calcium)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME} PRIVATE src)
