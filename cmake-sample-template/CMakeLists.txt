cmake_minimum_required(VERSION 3.15)

################################################
function(my_print  my_file my_line my_list)
	message("**************************")
	foreach(item ${my_list})
		message("sml: ${my_file} ${my_line}: ${item}")
	endforeach()
	message("**************************")
endfunction()


macro(my_log my_list)
	my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${my_list})
endmacro()


################################################
# project
set(MY_PROJECT_NAME cmake-sample-template)
project(${MY_PROJECT_NAME})

################################################
# usefull macros
my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${CMAKE_CURRENT_LIST_DIR})
my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${CMAKE_CURRENT_SOURCE_DIR})
my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${CMAKE_CURRENT_BINARY_DIR})
my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${PROJECT_SOURCE_DIR})
my_print(${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE} ${PROJECT_BINARY_DIR})



################################################
# project sources
set(MY_SOURCES "")
set(MY_FOUND_SOURCES "")

################################################# 
# enum sources for taking easy, 
# recomment to add sources mannually as cmake do good job on dependices

file(GLOB MY_FOUND_SOURCES ./*.h ./*.hpp)
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES ./*.cpp ./*.cxx)
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES ./*.c ./*.cc)
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES ./*.inc)
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

file(GLOB MY_FOUND_SOURCES ./*.asm ./*.s)
set(MY_SOURCES ${MY_SOURCES} ${MY_FOUND_SOURCES})

################################################
# add target exe or lib

add_executable(${MY_PROJECT_NAME} ${MY_SOURCES})
# add_library(${MY_PROJECT_NAME} SHARED ${MY_SOURCES})
set_target_properties(${MY_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


################################################
# target additional include directries
target_include_directories(${MY_PROJECT_NAME} PRIVATE "inc_dir1/inc1")
target_include_directories(${MY_PROJECT_NAME} PRIVATE "inc_dir2/inc2")
target_include_directories(${MY_PROJECT_NAME} PRIVATE "inc_dir3/inc3")

# target additional library directries
target_link_directories(${MY_PROJECT_NAME} PRIVATE "lib_dir1/lib1")
target_link_directories(${MY_PROJECT_NAME} PRIVATE "lib_dir2/lib2")
target_link_directories(${MY_PROJECT_NAME} PRIVATE "lib_dir3/lib3")

##################################################
# target additional library files

set(MY_EXTRA_LIBS "")

set(MY_EXTRA_LIBS ${MY_EXTRA_LIBS} "ws2_32.lib")
set(MY_EXTRA_LIBS ${MY_EXTRA_LIBS} "Wtsapi32.lib")
set(MY_EXTRA_LIBS ${MY_EXTRA_LIBS} "Netapi32.lib")

target_link_libraries(${MY_PROJECT_NAME} ${MY_EXTRA_LIBS})


##################################################
# install taeget and system libraries
install(TARGETS ${MY_PROJECT_NAME} DESTINATION bin)
include(installRequiredSystemLibraries)


##################################################
# add test 
#add_test()


##################################################
# cmake commands
# cmake-gui
# cmake --build . --config debug
# cmake --install . --config debug