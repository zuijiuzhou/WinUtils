cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(${THIRD_ARGPARSE_INCLUDE_DIR})

add_subdirectory(EnvVarEditor)

