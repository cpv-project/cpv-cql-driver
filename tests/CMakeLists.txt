cmake_minimum_required (VERSION 2.8.11)
project (CqlDriverTests)

FILE(GLOB_RECURSE Files ./*.cpp)
add_executable(${PROJECT_NAME} ${Files})

add_subdirectory(../3rd-party/googletest googletest EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} gtest_main CqlDriver)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

# get flags from cqldriver pkgconfig
set(CQLDRIVER_PC_PATH ../../bin/debug/cqldriver.pc)
execute_process(COMMAND pkg-config --cflags ${CQLDRIVER_PC_PATH} OUTPUT_VARIABLE CQLDRIVER_CFLAGS)
execute_process(COMMAND pkg-config --libs ${CQLDRIVER_PC_PATH} OUTPUT_VARIABLE CQLDRIVER_LDFLAGS)
string(STRIP "${CQLDRIVER_CFLAGS}" CQLDRIVER_CFLAGS)
string(STRIP "${CQLDRIVER_LDFLAGS}" CQLDRIVER_LDFLAGS)

# set compiler and link flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CQLDRIVER_CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CQLDRIVER_LDFLAGS}")

