cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(${THIRD_ARGPARSE_INCLUDE_DIR})

#一般通过命令行参数指定 --config Debug
# 未指定编译模式时默认Release
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
add_definitions(-DDEBUG -D_DEBUG)
endif()

add_subdirectory(EnvVarEditor)

