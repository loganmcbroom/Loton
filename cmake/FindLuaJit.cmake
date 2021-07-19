# Find LuaJit library
# 
# Variables Set
#  	LUAJIT_FOUND 				- true if found on the system
#  	LUAJIT_LIBRARY_DIR 		- full path to library
#  	LUAJIT_INCLUDE_DIR 		- include directory path
#
# Prefer interface: LuaJit::LuaJit
#

find_path( LUAJIT_INCLUDE_DIR
    NAME
		lua.h
	PATH_SUFFIXES
		src
		luajit/src
  )
  
find_library( LUAJIT_LIBRARY_DIR
    NAME
		libluajit-5.1.dll.a
		libluajit-5.1.dll.lib
	PATH_SUFFIXES
		src
		luajit/src
  )
  
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( LuaJit
	REQUIRED_VARS 
		LUAJIT_INCLUDE_DIR
		LUAJIT_LIBRARY_DIR
	HANDLE_COMPONENTS
	)

if( LUAJIT_FOUND )
	add_library( LuaJit::LuaJit INTERFACE IMPORTED )
	set_target_properties( LuaJit::LuaJit
		PROPERTIES 
			INTERFACE_INCLUDE_DIRECTORIES ${LUAJIT_INCLUDE_DIR}
			INTERFACE_LINK_LIBRARIES ${LUAJIT_LIBRARY_DIR}
	)
else()
	if( LUAJIT_FIND_REQUIRED )
		message( FATAL_ERROR "Couldn't find LuaJit" )
	endif()
endif()

mark_as_advanced(
    LUAJIT_INCLUDE_DIR
    LUAJIT_LIBRARY_DIR
    )
