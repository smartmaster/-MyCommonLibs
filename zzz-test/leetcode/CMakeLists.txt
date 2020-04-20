# do not change this file!!!!
# do not change this file!!!!
# do not change this file!!!!


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
include(CMakeLists.MY_PROJECT_VAR.cmake)

if(MY_CMAKE_TOOLCHAIN_FILE_VAR AND MY_TOOLCHAIN_PREFIX_VAR)
	set(CMAKE_TOOLCHAIN_FILE "${MY_CMAKE_TOOLCHAIN_FILE_VAR}" CACHE FILEPATH "CMAKE_TOOLCHAIN_FILE" FORCE)
endif()



set(MY_PROJECT_NAME ${MY_PROJECT_NAME_VAR})
project(${MY_PROJECT_NAME})

##############################################
# this file is rarely modified
# EDIT the CMakeLists.MY_VAR.cmake
include(CMakeLists.MY_VAR.cmake)

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
# my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${MY_USEFULL_MACROS})

################################################
# set default_build_type to current Debug
if(NOT MY_BUILD_INSTALL_TYPE)
	set(MY_BUILD_INSTALL_TYPE ${MY_BUILD_INSTALL_TYPE_VAR} CACHE STRING "My type of build" FORCE)
	set_property(CACHE MY_BUILD_INSTALL_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


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


################################################
set(MY_SUBDIRS ${MY_SUBDIRS_VAR})
foreach(MY_SUBDIR ${MY_SUBDIRS})
	add_subdirectory(${MY_SUBDIR})
endforeach()

################################################
my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} "To build ${MY_TARGET_TYPE_VAR}")
if(${MY_TARGET_TYPE_VAR} STREQUAL "NULL")
	my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} "Do not build any binary")

else() #build exe or lib

	################################################# 
	# project sources
	# enum sources for taking easy, 
	# recomment to add sources mannually as cmake do good job on dependices

	set(MY_SOURCES "")
	
	foreach(MY_SOURCE_PATTERN ${MY_SOURCE_PATTERNS_VAR})
		set(MY_FOUND_SOURCES "")
		file(GLOB MY_FOUND_SOURCES ${MY_SOURCE_PATTERN})
		set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})
	endforeach()


	################################################
	# enable asm language and set additional asm file extenstions
	if(MY_ASM_VAR AND MY_ASM_PATTERNS_VAR)
		enable_language(${MY_ASM_VAR})
		foreach(MY_ASM_PATTERN ${MY_ASM_PATTERNS_VAR})
			set(MY_ASM_SOURCES ${MY_SOURCES})
			list(FILTER MY_ASM_SOURCES INCLUDE REGEX ${MY_ASM_PATTERN})
			set_source_files_properties(${MY_ASM_SOURCES} PROPERTIES LANGUAGE ${MY_ASM_VAR})
		endforeach()
	endif()
	
	################################################
	# add project definitions
	if(MY_DEFINITIONS_VAR)
		add_definitions(${MY_DEFINITIONS_VAR})
	endif()

	################################################
	# add target executable or library

	if(${MY_TARGET_TYPE_VAR} STREQUAL "EXE")
		add_executable(${MY_PROJECT_NAME} ${MY_SOURCES})
	else()
		add_library(${MY_PROJECT_NAME} ${MY_TARGET_TYPE_VAR} ${MY_SOURCES})
	endif()


	set(MY_TARGET_PROPERTIES ${MY_TARGET_PROPERTIES_VAR})
	if(MY_TARGET_PROPERTIES)
		set_target_properties(${MY_PROJECT_NAME} PROPERTIES ${MY_TARGET_PROPERTIES})
	endif()


	if(MY_CMAKE_TOOLCHAIN_FILE_VAR AND MY_TOOLCHAIN_PREFIX_VAR)
		target_include_directories(${MY_PROJECT_NAME} PRIVATE "/usr/local/${MY_TOOLCHAIN_PREFIX_VAR}/include")
		target_include_directories(${MY_PROJECT_NAME} PRIVATE "/usr/${MY_TOOLCHAIN_PREFIX_VAR}/include")
		target_link_directories(${MY_PROJECT_NAME} PRIVATE "/usr/local/${MY_TOOLCHAIN_PREFIX_VAR}/lib")
		target_link_directories(${MY_PROJECT_NAME} PRIVATE "/usr/${MY_TOOLCHAIN_PREFIX_VAR}/lib")
	endif()
	

	################################################
	# target additional include directries
	set(MY_INC_DIRS ${MY_INC_DIRS_VAR})
	if(MY_INC_DIRS)
		target_include_directories(${MY_PROJECT_NAME} PRIVATE ${MY_INC_DIRS})
	endif()
	
	   
	################################################
	# target additional library directries
	set(MY_LIB_DIRS ${MY_LIB_DIRS_VAR})
	if(MY_LIB_DIRS)
		target_link_directories(${MY_PROJECT_NAME} PRIVATE ${MY_LIB_DIRS})
	endif()
	

	##################################################
	# target additional library files
	set(MY_EXTRA_LIBS ${MY_EXTRA_LIBS_VAR})
	if(MY_EXTRA_LIBS)
		target_link_libraries(${MY_PROJECT_NAME} PRIVATE ${MY_EXTRA_LIBS})
	endif()
	


	##################################################
	# target dependencies sp as to build them first
	set(MY_DEPENDENCIES ${MY_DEPENDENCIES_VAR})
	if(MY_DEPENDENCIES)
		add_dependencies(${MY_PROJECT_NAME} ${MY_DEPENDENCIES})
	endif()


	##################################################
	# install target and system libraries
	install(TARGETS ${MY_PROJECT_NAME} DESTINATION bin)
	include(InstallRequiredSystemLibraries)


	##################################################
	# add test 
	# add_test()


endif()  #build exe or lib