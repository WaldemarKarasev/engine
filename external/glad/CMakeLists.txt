cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(PROJECT_NAME glad)
project(${PROJECT_NAME})

add_library(glad STATIC include/glad/glad.h src/glad.c)
target_include_directories(${PROJECT_NAME} PUBLIC include/)