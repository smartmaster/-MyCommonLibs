# CMakeLists.txt 
# template and sample

# books
# https://cliutils.gitlab.io/modern-cmake/
# kitware "Mastering CMake"

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

##############################################
# EDIT the following line
set(MY_PROJECT_NAME_VAR "cmake-sample-template")

set(MY_BUILD_INSTALL_TYPE_VAR "Debug")

set(MY_SOURCE_PATTERNS_VAR

	"./*.h" "./*.hpp"

	"./*.cpp" "./*.cxx"
	"./*.c" "./*.cc"

	"./*.inc"
	"./*.asm" "./*.s"

	"./*.def"
)

set(MY_TARGET_PROPERTIES_VAR

	LANGUAGE CXX
	LINKER_LANGUAGE CXX

	CXX_STANDARD 17 
	CXX_STANDARD_REQUIRED YES
)

set(MY_TARGET_TYPE_VAR 
	# EXE
	SHARED
	# STATIC
	# MODULE
)


set(MY_INC_DIRS_VAR
	"inc_dir1/inc1"
	"inc_dir2/inc2"
	"inc_dir3/inc3"
)

set(MY_LIB_DIRS_VAR
	"lib_dir1/lib1"
	"lib_dir2/lib2"
	"lib_dir3/lib3"
)

if(WIN32)
	set(MY_EXTRA_LIBS_VAR
		"ws2_32"
		"Wtsapi32"
		"Netapi32"
	)
else()
	set(MY_EXTRA_LIBS_VAR
		""
		""
		""
	)
endif()

################################################
function(my_print MY_FILE MY_LINE)
	message("**************************")
	math(EXPR INDEX 0)
	foreach(ITEM ${ARGV})
		if(${INDEX} LESS 2)
			math(EXPR INDEX "${INDEX} + 1")
		else()
			message("sml: ${MY_FILE} ${MY_LINE}: ${ITEM}")
		endif()
	endforeach()
	message("**************************")
endfunction()


################################################
# project
set(MY_PROJECT_NAME ${MY_PROJECT_NAME_VAR})
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
# set default_build_type to current Debug
if(NOT MY_BUILD_INSTALL_TYPE)
	set(MY_BUILD_INSTALL_TYPE ${MY_BUILD_INSTALL_TYPE_VAR} CACHE STRING "My type of build" FORCE)
	set_property(CACHE MY_BUILD_INSTALL_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

#if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#if(MY_BUILD_INSTALL_TYPE)
if(MY_BUILD_INSTALL_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	# Set the cache entry CMAKE_BUILD_TYPE to default build type for cmake-gui
    set(CMAKE_BUILD_TYPE ${MY_BUILD_INSTALL_TYPE} CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

################################################
# set CMAKE_INSTALL_PREFIX to current PROJECT_BINARY_DIR
if(MY_BUILD_INSTALL_TYPE AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install/${MY_BUILD_INSTALL_TYPE}" CACHE PATH "install to current project bin folder" FORCE)
	set(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT FALSE CACHE BOOL "cmake_install_prefix_initialized_to_default" FORCE)
endif()




################################################# 
# project sources
# enum sources for taking easy, 
# recomment to add sources mannually as cmake do good job on dependices

set(MY_SOURCES "")
set(MY_FOUND_SOURCES "")

set(MY_SOURCE_PATTERNS ${MY_SOURCE_PATTERNS_VAR})
file(GLOB MY_FOUND_SOURCES ${MY_SOURCE_PATTERNS})
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

################################################
# enable asm language and set additional asm file extenstions
if(WIN32)
	enable_language(ASM_MASM)

	# set additional asm file extenstions
	set(MY_ASM_SOURCES ${MY_SOURCES})
	list(FILTER MY_ASM_SOURCES INCLUDE REGEX ".+\.asm$")
	set_source_files_properties(${MY_ASM_SOURCES} PROPERTIES LANGUAGE ASM_MASM)


	set(MY_ASM_SOURCES ${MY_SOURCES})
	list(FILTER MY_ASM_SOURCES INCLUDE REGEX ".+\.s$")
	set_source_files_properties(${MY_ASM_SOURCES} PROPERTIES LANGUAGE ASM_MASM)
else()
	enable_language(ASM)

	# set additional asm file extenstions
	set(MY_ASM_SOURCES ${MY_SOURCES})
	list(FILTER MY_ASM_SOURCES INCLUDE REGEX ".+\.asm$")
	set_source_files_properties(${MY_ASM_SOURCES} PROPERTIES LANGUAGE ASM)

	set(MY_ASM_SOURCES ${MY_SOURCES})
	list(FILTER MY_ASM_SOURCES INCLUDE REGEX ".+\.s$")
	set_source_files_properties(${MY_ASM_SOURCES} PROPERTIES LANGUAGE ASM)
endif()


################################################
# add target executable or library

if(${MY_TARGET_TYPE_VAR} STREQUAL "EXE")
add_executable(${MY_PROJECT_NAME} ${MY_SOURCES})
else()
add_library(${MY_PROJECT_NAME} ${MY_TARGET_TYPE_VAR} ${MY_SOURCES})
endif()


set(MY_TARGET_PROPERTIES ${MY_TARGET_PROPERTIES_VAR})
set_target_properties(${MY_PROJECT_NAME} PROPERTIES ${MY_TARGET_PROPERTIES})

################################################
# target additional include directries
set(MY_INC_DIRS ${MY_INC_DIRS_VAR})
target_include_directories(${MY_PROJECT_NAME} PRIVATE ${MY_INC_DIRS})



################################################
# target additional library directries
set(MY_LIB_DIRS ${MY_LIB_DIRS_VAR})
target_link_directories(${MY_PROJECT_NAME} PRIVATE ${MY_LIB_DIRS})

##################################################
# target additional library files
set(MY_EXTRA_LIBS ${MY_EXTRA_LIBS_VAR})
target_link_libraries(${MY_PROJECT_NAME} PRIVATE ${MY_EXTRA_LIBS})



##################################################
# install target and system libraries
install(TARGETS ${MY_PROJECT_NAME} DESTINATION bin)
include(InstallRequiredSystemLibraries)


##################################################
# add test 
# add_test()



