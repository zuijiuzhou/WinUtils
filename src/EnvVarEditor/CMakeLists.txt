cmake_minimum_required(VERSION 3.10)

set(CURRENT_PROJ_NAME EnvVarEditor)
# set(CMAKE_MFC_FLAG 1)

FILE(GLOB SRC_HEADER "./*.h")
FILE(GLOB SRC_CPP "./*.cpp")
FILE(GLOB SRC_RC "./*.rc")

set(CMAKE_PREFIX_PATH ${BOOST_DIR})
find_package(Boost 1.81 REQUIRED)

add_definitions(-DUNICODE -D_UNICODE)
# add_definitions(-D_AFXDLL)

add_executable(${CURRENT_PROJ_NAME} ${SRC_HEADER} ${SRC_CPP} ${SRC_RC})
target_include_directories(${CURRENT_PROJ_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_directories(${CURRENT_PROJ_NAME} PUBLIC ${Boost_LIBRARY_DIRS})
# set_target_properties(${CURRENT_PROJ_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")