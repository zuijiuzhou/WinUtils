cmake_minimum_required(VERSION 3.12)

project(
    WinUtils 
    VERSION 1.0.0  
    HOMEPAGE_URL "https://github.com/zuijiuzhou/WinUtils" 
    LANGUAGES CXX C)

set(THIRD_DIR ${CMAKE_SOURCE_DIR}/third)
set(THIRD_ARGPARSE_DIR ${THIRD_DIR}/argparse)
set(THIRD_ARGPARSE_INCLUDE_DIR ${THIRD_ARGPARSE_DIR}/include)

add_subdirectory(src)