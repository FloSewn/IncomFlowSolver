#***********************************************************
#
#***********************************************************
cmake_minimum_required(VERSION 3.10)
project(IncomFlow
        LANGUAGES CXX
        VERSION   1.1)

# Set C++ flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wunused-variable -std=c++17 -O3 -DNDEBUG -pg -fconcepts")
#set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wunused-variable -std=c++17 -O3 -DNDEBUG -fconcepts")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++17 -O0 -g -pg -fconcepts")
#set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wunused-variable -std=c++17 -O0 -g -fconcepts")
#set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wunused-variable -std=c++17 -O0 -g -fconcepts -DNDEBUG")

set(CMAKE_CXX_COMPILER "g++-10")

set( BIN ${CMAKE_SOURCE_DIR}/bin )

# Add config file
configure_file(aux/IncomFlowConfig.h.in ${CMAKE_BINARY_DIR}/IncomFlowConfig.h)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory( src/utils )
add_subdirectory( src/extern_libs )
add_subdirectory( src/solver )
add_subdirectory( src/tests )


#---------------------------------------------------------------------
# Testing
#---------------------------------------------------------------------
enable_testing()

add_test(NAME PrimaryGrid COMMAND ${CMAKE_SOURCE_DIR}/bin/run_tests "PrimaryGrid")