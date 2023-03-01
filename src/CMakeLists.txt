cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(${THIRD_ARGPARSE_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${LIBRARY_OUTPUT_PATH})
link_libraries("cutil.lib")
#一般通过命令行参数指定 --config Debug
# 未指定编译模式时默认Release
# if(NOT CMAKE_BUILD_TYPE)
# message("___RELEASE___")
# set(CMAKE_BUILD_TYPE "Release")
# endif()

# if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
# message("___DEBUG___")
# add_definitions(-DDEBUG -D_DEBUG)
# endif()

add_subdirectory(cutil)
add_subdirectory(EnvVarEditor)

