cmake_minimum_required(VERSION 3.27)
project(day9 C)
set(CMAKE_C_STANDARD 17)

include(CTest)

add_executable(day9 ${PROJECT_SOURCE_DIR}/main.c aoclib.c
        aocheap.c
        aocheap.h
        aocmap.c
        aocmap.h
        aoclist.c
        aoclist.h)
