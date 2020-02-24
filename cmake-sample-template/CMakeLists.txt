# CMakeLists.txt 
# template and sample

# books
# https://cliutils.gitlab.io/modern-cmake/
# kiware "Mastering CMake"

##################################################
# cmake commands

# cmake-gui

# for Windows to build and install debug or releae configuration
# cmake --build . --config debug
# cmake --install . --config debug
# cmake --build . --config release
# cmake --install . --config release

# for nix to build and install debug or releae configuration
# specify cache entry CMAKE_BUILD_TYPE to debug or release 
# cmake --build . 
# cmake --install .

cmake_minimum_required(VERSION "3.15")

################################################
function(my_print my_file my_line)
	message("**************************")
	math(EXPR INDEX 0)
	foreach(item ${ARGV})
		if(${INDEX} LESS 2)
			math(EXPR INDEX "${INDEX} + 1")
		else()
			message("sml: ${my_file} ${my_line}: ${item}")
		endif()
	endforeach()
	message("**************************")
endfunction()


################################################
# project
set(MY_PROJECT_NAME "cmake-sample-template")
project(${MY_PROJECT_NAME})


################################################
# usefull macros
set(MY_USEFULL_MACROS

	"CMAKE_CURRENT_LIST_***"
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_FILE}
	${CMAKE_CURRENT_LIST_LINE}

	"CMAKE_CURRENT_***_DIR"
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}


	"PROJECT_***_DIR"
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}

	"CMAKE_CONFIGURATION_TYPES***"
	${CMAKE_CONFIGURATION_TYPES}

	"CMAKE_BUILD_TYPE***"
	${CMAKE_BUILD_TYPE}
)
my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${MY_USEFULL_MACROS})

################################################
# set CMAKE_INSTALL_PREFIX to current PROJECT_BINARY_DIR
if(${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})
	set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE PATH "install to current project bin folder" FORCE)
endif()



################################################
# set default_build_type to current Debug
set(MY_DEFAULT_BUILD_TYPE "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE ${MY_DEFAULT_BUILD_TYPE} CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()



################################################
# project sources
set(MY_SOURCES "")
set(MY_FOUND_SOURCES "")

################################################# 
# enum sources for taking easy, 
# recomment to add sources mannually as cmake do good job on dependices

file(GLOB MY_FOUND_SOURCES "./*.h" "./*.hpp")
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES "./*.cpp" "./*.cxx")
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES "./*.c" "./*.cc")
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES "./*.inc")
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES "./*.asm" "./*.s")
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})


file(GLOB MY_FOUND_SOURCES "./*.def")
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

################################################
# enable asm language and set additional asm file extenstions
enable_language(ASM_MASM)

# set additional asm file extenstions
set(MY_ASM_SOURCES ${MY_SOURCES})
list(FILTER MY_ASM_SOURCES INCLUDE REGEX ".+\.s$")
set_source_files_properties(${MY_ASM_SOURCES} PROPERTIES LANGUAGE ASM_MASM)


################################################
# add target executable or library

if(OFF)
add_executable(${MY_PROJECT_NAME} ${MY_SOURCES})
else()
add_library(${MY_PROJECT_NAME} SHARED ${MY_SOURCES})
endif()

set_target_properties(${MY_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


################################################
# target additional include directries
set(MY_INC_DIRS 
	"inc_dir1/inc1"
	"inc_dir2/inc2"
	"inc_dir3/inc3"
)
target_include_directories(${MY_PROJECT_NAME} PRIVATE ${MY_INC_DIRS})



################################################
# target additional library directries
set(MY_LIB_DIRS
	"lib_dir1/lib1"
	"lib_dir2/lib2"
	"lib_dir3/lib3"
)
target_link_directories(${MY_PROJECT_NAME} PRIVATE ${MY_LIB_DIRS})

##################################################
# target additional library files
set(MY_EXTRA_LIBS
	"ws2_32.lib"
	"Wtsapi32.lib"
	"Netapi32.lib"
)
target_link_libraries(${MY_PROJECT_NAME} ${MY_EXTRA_LIBS})



##################################################
# install taeget and system libraries
install(TARGETS ${MY_PROJECT_NAME} DESTINATION bin)
include(InstallRequiredSystemLibraries)


##################################################
# add test 
# add_test()



