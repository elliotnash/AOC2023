cmake_minimum_required(VERSION 3.27)
project(day6 C)
set(CMAKE_C_STANDARD 17)

include(CTest)

add_executable(day6 ${PROJECT_SOURCE_DIR}/main.c aoclib.c)
