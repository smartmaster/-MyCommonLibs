#!/bin/bash

set cmake_build ..\cmake-sample-template-build-windows
set cmake_config debug

cmake --install $cmake_build --config $cmake_config


