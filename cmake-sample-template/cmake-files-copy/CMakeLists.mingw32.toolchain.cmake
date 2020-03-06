# do not change this file!!!!
# do not change this file!!!!
# do not change this file!!!!

################################################################################
# https://github.com/ROCmSoftwarePlatform/MIOpen/blob/master/cmake/mingw-toolchain.cmake
# https://github.com/RobertBeckebans/RBDOOM-3-BFG/issues/340
#
# MIT License
# 
# Copyright (c) 2017 Advanced Micro Devices, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# 
################################################################################


# set(MY_TOOLCHAIN_PREFIX_VAR "x86_64-w64-mingw32")
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# cross compilers to use for C and C++
set(CMAKE_C_COMPILER ${MY_TOOLCHAIN_PREFIX_VAR}-gcc)
set(CMAKE_CXX_COMPILER ${MY_TOOLCHAIN_PREFIX_VAR}-g++)
set(CMAKE_RC_COMPILER ${MY_TOOLCHAIN_PREFIX_VAR}-windres)
# set(CMAKE_C_LINK_EXECUTABLE ${MY_TOOLCHAIN_PREFIX_VAR}-ld)
# set(CMAKE_AR ${MY_TOOLCHAIN_PREFIX_VAR}-ar)

# target environment on the build host system
#   set 1st to dir with the cross compiler's C/C++ headers/libs
#   set 2nd to dir containing personal cross development headers/libs
set(CMAKE_FIND_ROOT_PATH "/usr/local/${MY_TOOLCHAIN_PREFIX_VAR}" "/usr/${MY_TOOLCHAIN_PREFIX_VAR}")

# modify default behavior of FIND_XXX() commands to
# search for headers/libs in the target environment and
# search for programs in the build host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# include_directories("/usr/local/${MY_TOOLCHAIN_PREFIX_VAR}/include")
# include_directories("/usr/${MY_TOOLCHAIN_PREFIX_VAR}/include")

# link_directories("/usr/local/${MY_TOOLCHAIN_PREFIX_VAR}/lib")
# link_directories("/usr/${MY_TOOLCHAIN_PREFIX_VAR}/lib")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON CACHE BOOL "Export windows symbols")

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -static-libgcc -static-libstdc++" CACHE STRING "static link c/cxx libraries")



