# CMake version last checked 26-09-2020 (Version=3.14.5)
cmake_minimum_required(VERSION 3.14)

# Project Name
project(miotywaffles)

# Set Compiler Standard
set(CMAKE_CXX_STANDARD 14)

# Header files are kept with other files in src
include_directories(src)

# Sub Directories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)