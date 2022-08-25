#***********************************************************
# Module: solver
#***********************************************************
set( MODULE_MESH solver )

add_library( ${MODULE_MESH} INTERFACE )

target_include_directories( ${MODULE_MESH}
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

target_link_libraries( ${MODULE_MESH}
  INTERFACE m
  INTERFACE extern_libs
  INTERFACE util )
