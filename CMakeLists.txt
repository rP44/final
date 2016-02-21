cmake_minimum_required(VERSION 2.8)
project(final)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64 -Ofast -march=native -funroll-loops")

set(SOURCE_LIB foo.cpp)
add_library(foo STATIC ${SOURCE_LIB})

set(SOURCE_EXE main.cpp)
add_executable(final ${SOURCE_EXE})

target_link_libraries(final foo pthread)

