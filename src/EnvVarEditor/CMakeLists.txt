cmake_minimum_required(VERSION 3.10)

set(CURRENT_PROJ_NAME EnvVarEditor)
# set(CMAKE_MFC_FLAG 1)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DCUITL_STATIC)
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

FILE(GLOB SRC_HEADER "./*.h")
FILE(GLOB SRC_CPP "./*.cpp")
FILE(GLOB SRC_RC "./*.rc")

option(OPT_BOOST_ENABLED "是否启用BOOST" OFF)

set(OPT_BOOST_ENABLED OFF)


# add_definitions(-D_AFXDLL)

add_executable(${CURRENT_PROJ_NAME} ${SRC_HEADER} ${SRC_CPP} ${SRC_RC})
set_property(TARGET ${CURRENT_PROJ_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_target_properties(${CURRENT_PROJ_NAME} PROPERTIES LINK_FLAGS "/ignore:4099")
if(OPT_BOOST_ENABLED)
set(CMAKE_PREFIX_PATH $ENV{LIB_BOOST_DIR})
find_package(Boost 1.81 REQUIRED)
target_include_directories(${CURRENT_PROJ_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_directories(${CURRENT_PROJ_NAME} PUBLIC ${Boost_LIBRARY_DIRS})
add_definitions(-D__BOOST_ENABLED__)

endif()


# set_target_properties(${CURRENT_PROJ_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")