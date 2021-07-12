# Find Qwt library
# 
# Variables Set
#  	QWT_FOUND 				- true if found on the system
#  	QWT_LIBRARY_DIR 		- full path to library
#  	QWT_INCLUDE_DIR 		- include directory path
#
# Prefer interface: Qwt::Qwt
#

find_path( QWT_INCLUDE_DIR
    NAME
		qwt.h
	PATH_SUFFIXES
		src
		qwt/src
		qwt-6.1.6/src
  )

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
	find_library( QWT_LIBRARY_DIR
		NAME
			qwt.lib
		PATH_SUFFIXES
			lib
			qwt/lib
			qwt-6.1.6/lib
	  )
endif()
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	find_library( QWT_LIBRARY_DIR
		NAME
			qwtd.lib
		PATH_SUFFIXES
			lib
			qwt/lib
			qwt-6.1.6/lib
	  )
endif()
  
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Qwt
	REQUIRED_VARS 
		QWT_INCLUDE_DIR
		QWT_LIBRARY_DIR
	HANDLE_COMPONENTS
	)

if( QWT_FOUND )
	add_library( Qwt::Qwt INTERFACE IMPORTED )
	set_target_properties( Qwt::Qwt
		PROPERTIES 
			INTERFACE_INCLUDE_DIRECTORIES ${QWT_INCLUDE_DIR}
			INTERFACE_LINK_LIBRARIES ${QWT_LIBRARY_DIR}
	)
else()
	if( QWT_FIND_REQUIRED )
		message( FATAL_ERROR "Couldn't find Qwt" )
	endif()
endif()

mark_as_advanced(
    QWT_INCLUDE_DIR
    QWT_LIBRARY_DIR
    )
