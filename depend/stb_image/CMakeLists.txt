cmake_minimum_required(VERSION 3.0)

project(stb_image)

add_library(${PROJECT_NAME} stb_image.c)

target_include_directories(${PROJECT_NAME} PUBLIC .)
