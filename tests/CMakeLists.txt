cmake_minimum_required (VERSION 2.8.11)
project (CqlDriverTests)

FILE(GLOB_RECURSE Files ./*.cpp)
FILE(GLOB_RECURSE PublicHeaders ../include/*.hpp)
FILE(GLOB_RECURSE InternalHeaders ../src/*.hpp)
add_executable(${PROJECT_NAME} ${Files} ${PublicHeaders} ${InternalHeaders})

# set compile options, libCqlDriver should specific as a path for modification detect
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(LIBCQLDRIVER_PATH "${CMAKE_SOURCE_DIR}/../bin/debug/libCqlDriver.a")
add_subdirectory(../3rd-party/googletest googletest EXCLUDE_FROM_ALL)
include_directories(../src)
target_link_libraries(${PROJECT_NAME} gtest_main ${LIBCQLDRIVER_PATH})

# add predefined macros
add_definitions(-DDB_SIMPLE_IP="127.0.0.1")
add_definitions(-DDB_SIMPLE_PORT=9043)
add_definitions(-DDB_SSL_1_IP="127.0.0.1")
add_definitions(-DDB_SSL_1_PORT=9044)
add_definitions(-DDB_SSL_2_IP="127.0.0.1")
add_definitions(-DDB_SSL_2_PORT=9045)

# get flags from cqldriver pkgconfig
set(CQLDRIVER_PC_PATH ../../bin/debug/cqldriver.pc)
execute_process(COMMAND pkg-config --cflags ${CQLDRIVER_PC_PATH} OUTPUT_VARIABLE CQLDRIVER_CFLAGS)
execute_process(COMMAND pkg-config --libs ${CQLDRIVER_PC_PATH} OUTPUT_VARIABLE CQLDRIVER_LDFLAGS)
string(STRIP "${CQLDRIVER_CFLAGS}" CQLDRIVER_CFLAGS)
string(STRIP "${CQLDRIVER_LDFLAGS}" CQLDRIVER_LDFLAGS)

# set compiler and link flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CQLDRIVER_CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CQLDRIVER_LDFLAGS}")

