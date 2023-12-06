cmake_minimum_required(VERSION 3.27)
project(day5 C)
set(CMAKE_C_STANDARD 17)

include(CTest)

add_executable(day5 ${PROJECT_SOURCE_DIR}/main.c aoclib.c)
