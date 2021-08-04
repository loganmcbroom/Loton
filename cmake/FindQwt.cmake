# Find Qwt library
# 
# Variables Set
#  	Qwt_FOUND 				- true if found on the system
#  	Qwt_LIBRARY_DIR 		- full path to library
#  	Qwt_INCLUDE_DIR 		- include directory path
#
# Prefer interface: Qwt::Qwt
#

find_package( PkgConfig )
pkg_check_modules( PC_Qwt QUIET Qwt )

find_path( Qwt_INCLUDE_DIR
    NAMES qwt.h
	PATHS ${PC_Qwt_INCLUDE_DIRS}
	PATH_SUFFIXES
		src
		qwt/src
		qwt-6.1.6/src
  )
 
find_library( Qwt_LIBRARY_RELEASE
	NAME qwt
	PATHS ${PC_Qwt_LIBRARY_DIRS} ${PC_Qwt_LIBRARY_DIRS}/Release
	PATH_SUFFIXES
		lib
		qwt/lib
		qwt-6.1.6/lib
  )

find_library( Qwt_LIBRARY_DEBUG
	NAME qwtd
	PATHS ${PC_Qwt_LIBRARY_DIRS} ${PC_Qwt_LIBRARY_DIRS}/Debug
	PATH_SUFFIXES
		lib
		qwt/lib
		qwt-6.1.6/lib
  )
 
# Handle Debug/Release builds
include( SelectLibraryConfigurations )
select_library_configurations( Qwt )
  
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Qwt
	FOUND_VAR Qwt_FOUND
	REQUIRED_VARS 
		Qwt_LIBRARY
		Qwt_INCLUDE_DIR
	VERSION_VAR Qwt_VERSION
	)

if( Qwt_FOUND )
	if( NOT TARGET Qwt::Qwt )
		add_library( Qwt::Qwt UNKNOWN IMPORTED )
	endif()

	if( Qwt_LIBRARY_RELEASE )
		set_property( TARGET Qwt::Qwt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE )
		set_target_properties( Qwt::Qwt PROPERTIES IMPORTED_LOCATION_RELEASE "${Qwt_LIBRARY_RELEASE}" )
	endif()

	if( Qwt_LIBRARY_DEBUG )
		set_property( TARGET Qwt::Qwt APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG )
		set_target_properties( Qwt::Qwt PROPERTIES IMPORTED_LOCATION_DEBUG "${Qwt_LIBRARY_DEBUG}" )
	endif()

	set_target_properties( Qwt::Qwt PROPERTIES
		INTERFACE_COMPILE_OPTIONS "${PC_Qwt_CFLAGS_OTHER}"
		INTERFACE_INCLUDE_DIRECTORIES "${Qwt_INCLUDE_DIR}"
	)
else()
	if( QWT_FIND_REQUIRED )
		message( FATAL_ERROR "Couldn't find Qwt" )
	endif()
endif()

mark_as_advanced(
    Qwt_INCLUDE_DIR
    Qwt_LIBRARY
    )
