cmake_minimum_required (VERSION 2.8.11)
project (CQLDriver)

FILE(GLOB_RECURSE Files ./*.cpp)
FILE(GLOB_RECURSE PublicHeaders ../include/*.hpp)
FILE(GLOB_RECURSE InternalHeaders ../src/*.hpp)
add_library(${PROJECT_NAME} STATIC ${Files} ${PublicHeaders} ${InternalHeaders})

# set compile options
set(CMAKE_VERBOSE_MAKEFILE TRUE)
include_directories(../include)
target_link_libraries(${PROJECT_NAME})

# get flags from seastar pkgconfig
if (CMAKE_BUILD_TYPE MATCHES Debug)
	set(SEASTAR_PC_PATH ../../3rd-party/seastar/build/debug/seastar.pc)
else()
	set(SEASTAR_PC_PATH ../../3rd-party/seastar/build/release/seastar.pc)
endif()
execute_process(COMMAND pkg-config --cflags ${SEASTAR_PC_PATH} OUTPUT_VARIABLE SEASTAR_CFLAGS)
execute_process(COMMAND pkg-config --libs ${SEASTAR_PC_PATH} OUTPUT_VARIABLE SEASTAR_LDFLAGS)
string(STRIP "${SEASTAR_CFLAGS}" SEASTAR_CFLAGS)
string(STRIP "${SEASTAR_LDFLAGS}" SEASTAR_LDFLAGS)
string(REPLACE "-Werror" "" SEASTAR_CFLAGS "${SEASTAR_CFLAGS}")
string(REPLACE "-Werror" "" SEASTAR_LDFLAGS "${SEASTAR_LDFLAGS}")

# set compiler and link flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-function -Wno-noexcept-type ${SEASTAR_CFLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto ${SEASTAR_LDFLAGS}")

# generate pkgconfig file for cqldriver
file(READ ../VERSION VERSION_NUMBER)
string(STRIP "${VERSION_NUMBER}" VERSION_NUMBER)
string(TIMESTAMP BUILD_DATE)
set(PC_PATH "${CMAKE_BINARY_DIR}/cqldriver.pc")
get_filename_component(TOP_PATH ${CMAKE_SOURCE_DIR} PATH)
file(REMOVE ${PC_PATH})
file(APPEND ${PC_PATH} "Name: CQLDriver\n")
file(APPEND ${PC_PATH} "URL: https://github.com/cpv-project/cpv-cql-driver\n")
file(APPEND ${PC_PATH} "Description: C++ database driver for cassandra and scylla, build date: ${BUILD_DATE}\n")
file(APPEND ${PC_PATH} "Version: ${VERSION_NUMBER}\n")
file(APPEND ${PC_PATH} "Cflags: ${CMAKE_CXX_FLAGS} -I'${TOP_PATH}/include'\n")
file(APPEND ${PC_PATH} "Libs: ${CMAKE_SHARED_LINKER_FLAGS} -L'${CMAKE_BINARY_DIR}' -lCQLDriver\n")

