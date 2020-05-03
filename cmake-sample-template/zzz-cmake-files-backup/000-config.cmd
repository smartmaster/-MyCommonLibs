
set CMAKE_SOURCE=%cd%

set CMAKE_BUILD=%cd%-out-win

mkdir "%CMAKE_BUILD%"
cmake-gui -S "%CMAKE_SOURCE%" -B "%CMAKE_BUILD%"



