# CMakeList.txt : CMake project for 000-cmake-gen, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source to this project's executable.
add_executable (000-cmake-gen "000-cmake-gen.cpp" "000-cmake-gen.h")

# TODO: Add tests and install targets if needed.
