#!/bin/bash

set cmake_source .
set cmake_build ..\cmake-sample-template-build-windows

mkdir $cmake_build 
cmake-gui -S $cmake_source -B $cmake_build




