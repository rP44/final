cmake_minimum_required(VERSION 2.8)
project(final)

#release
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m64 -Ofast -march=native -funroll-loops")

#debug
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")

set(SOURCE_LIB helper_functions.cpp)
add_library(helper_functions STATIC ${SOURCE_LIB})

set(SOURCE_EXE main.cpp)
add_executable(final ${SOURCE_EXE})

target_link_libraries(final helper_functions pthread)

