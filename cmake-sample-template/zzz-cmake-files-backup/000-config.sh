#!/bin/sh

CMAKE_SOURCE=$(pwd)
CMAKE_BUILD=$(pwd)-out-nix


mkdir "$CMAKE_BUILD"
cmake-gui -S "$CMAKE_SOURCE" -B "$CMAKE_BUILD"





