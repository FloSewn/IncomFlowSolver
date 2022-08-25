set( TESTS run_tests )

add_executable( ${TESTS}
  tests_PrimaryGrid.cpp
  tests.cpp
  main.cpp
)

target_link_libraries( ${TESTS}
  util
  solver
)

install( TARGETS ${TESTS} RUNTIME DESTINATION ${BIN} )
