cmake_minimum_required (VERSION 2.8.11)
project (CqlDriver)

FILE(GLOB_RECURSE Files ./*.cpp)
add_library(${PROJECT_NAME} STATIC ${Files})

include_directories(../include)
target_link_libraries(${PROJECT_NAME})
set(CMAKE_VERBOSE_MAKEFILE TRUE)

execute_process(
	COMMAND pkg-config --cflags --libs ../../3rd-party/seastar/build/debug/seastar.pc
	OUTPUT_VARIABLE SEASTAR_DEBUG_FLAGS)
execute_process(
	COMMAND pkg-config --cflags --libs ../../3rd-party/seastar/build/release/seastar.pc
	OUTPUT_VARIABLE SEASTAR_RELEASE_CFLAGS)
set(COMMON_CFLAGS "-Wno-unused-variable")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_CFLAGS} ${SEASTAR_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_CFLAGS} ${SEASTAR_RELEASE_CFLAGS}")

