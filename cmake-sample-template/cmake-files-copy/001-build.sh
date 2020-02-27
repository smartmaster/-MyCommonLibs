#!/bin/sh

CMAKE_BUILD=$(pwd)-out-nix
CMAKE_CONFIG=debug


cmake --build "$CMAKE_BUILD" --config $CMAKE_CONFIG



