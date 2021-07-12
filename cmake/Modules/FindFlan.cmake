# Find Flan library
# 
# Variables Set
#  	FLAN_FOUND 				- true if found on the system
#  	FLAN_LIBRARY_DIR 		- full path to library
#  	FLAN_INCLUDE_DIR 		- include directory path
#
# Prefer interface: Flan::Flan
#

find_path( FLAN_INCLUDE_DIR
    NAME
		flan
	PATH_SUFFIXES
		flan/include
  )

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
	find_library( FLAN_LIBRARY_DIR
		NAME
			flan.lib
		PATH_SUFFIXES
			flan/lib
		) 
endif()
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	find_library( FLAN_LIBRARY_DIR
		NAME
			fland.lib
		PATH_SUFFIXES
			flan/lib
	) 
endif()
  
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Flan
	REQUIRED_VARS 
		FLAN_INCLUDE_DIR
		FLAN_LIBRARY_DIR
	HANDLE_COMPONENTS
	)

if( FLAN_FOUND )
	add_library( Flan::Flan INTERFACE IMPORTED )
	set_target_properties( Flan::Flan
		PROPERTIES 
			INTERFACE_INCLUDE_DIRECTORIES ${FLAN_INCLUDE_DIR}
			INTERFACE_LINK_LIBRARIES ${FLAN_LIBRARY_DIR}
	)
else()
	if( FLAN_FIND_REQUIRED )
		message( FATAL_ERROR "Couldn't find Flan" )
	endif()
endif()

mark_as_advanced(
    FLAN_INCLUDE_DIR
    FLAN_LIBRARY_DIR
    )
