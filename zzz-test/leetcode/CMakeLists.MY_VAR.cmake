# make change on this file!!!!
# make change on this file!!!!
# make change on this file!!!!



# "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
# "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
set(MY_BUILD_INSTALL_TYPE_VAR "Debug")

# NULL EXE SHARED STATIC MODULE
# NULL EXE SHARED STATIC MODULE
set(MY_TARGET_TYPE_VAR 
	EXE 
)

set(MY_SOURCE_PATTERNS_VAR

	"${PROJECT_SOURCE_DIR}/src/*.h"

	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	
)

# ASM_MASM ASM
# ASM_MASM ASM
if(WIN32)

#set(MY_ASM_VAR "")
#set(MY_ASM_VAR ASM)
set(MY_ASM_VAR ASM_MASM)

else()

set(MY_ASM_VAR "")
#set(MY_ASM_VAR ASM)

endif()

# ".+\.asm$" ".+\.s$"
# ".+\.asm$" ".+\.s$"
set(MY_ASM_PATTERNS_VAR
	".+\.asm$"
	".+\.s$"
)
#set(CMAKE_MSVC_RUNTIME_LIBRARY "")

# LANGUAGE CXX
# LINKER_LANGUAGE CXX
# CXX_STANDARD 17 
# CXX_STANDARD_REQUIRED YES
set(MY_TARGET_PROPERTIES_VAR

	LANGUAGE CXX
	LINKER_LANGUAGE CXX

	CXX_STANDARD 17 
	CXX_STANDARD_REQUIRED YES
)

if(WIN32)
set(MY_DEFINITIONS_VAR
	-DUNICODE
	-D_UNICODE
)
else()
set(MY_DEFINITIONS_VAR
	""
	""
)
endif()


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
		"rpcrt4"
		""
	)
else()
	set(MY_EXTRA_LIBS_VAR
		"cap"
		"acl"
		""
	)
endif()


set(MY_DEPENDENCIES_VAR
	""
	""
)

set(MY_SUBDIRS_VAR
	""
	""
)

