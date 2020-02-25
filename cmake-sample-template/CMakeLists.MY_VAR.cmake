


set(MY_BUILD_INSTALL_TYPE_VAR "Debug")

# NULL EXE SHARED STATIC MODULE
# NULL EXE SHARED STATIC MODULE
set(MY_TARGET_TYPE_VAR 
	EXE
)

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



set(MY_SUBDIRS_VAR
	""
)

