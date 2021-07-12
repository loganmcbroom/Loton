# Find NodeEditor library
#
# Variables Set
#   NODEEDITOR_FOUND                  - true if found on the system
#   NODEEDITOR_LIBRARY_DIR        	  - full path to library
#   NODEEDITOR_INCLUDE_DIR            - include directory path
#
# Prefer interface: NodeEditor::NodeEditor
#

find_path( NODEEDITOR_INCLUDE_DIR
    NAMES
		nodes
	PATH_SUFFIXES
		nodeeditor/include
  )

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
	find_library( NODEEDITOR_LIBRARY_DIR
		NAME
			nodes.lib
		PATH_SUFFIXES
			lib
			nodeeditor/lib
		) 
endif()
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	find_library( NODEEDITOR_LIBRARY_DIR
		NAME
			nodesd.lib
		PATH_SUFFIXES
			lib
			nodeeditor/lib
		) 
endif()

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( NodeEditor
	REQUIRED_VARS 
		NODEEDITOR_INCLUDE_DIR
		NODEEDITOR_LIBRARY_DIR
	HANDLE_COMPONENTS
	)

if( NODEEDITOR_FOUND )
	add_library( NodeEditor::NodeEditor INTERFACE IMPORTED )
	set_target_properties( NodeEditor::NodeEditor
		PROPERTIES 
			INTERFACE_INCLUDE_DIRECTORIES ${NODEEDITOR_INCLUDE_DIR}
			INTERFACE_LINK_LIBRARIES ${NODEEDITOR_LIBRARY_DIR}
	)
else()
	if( NODEEDITOR_FIND_REQUIRED )
		message( FATAL_ERROR "Couldn't find NodeEditor" )
	endif()
endif()

mark_as_advanced(
    NODEEDITOR_INCLUDE_DIR
    NODEEDITOR_LIBRARY_DIR
    )