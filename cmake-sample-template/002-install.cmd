
set CMAKE_BUILD=%cd%-out-win

set CMAKE_CONFIG=debug

cmake --install "%CMAKE_BUILD%" --config "%CMAKE_CONFIG%"

