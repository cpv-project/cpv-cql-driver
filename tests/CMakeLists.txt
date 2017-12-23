cmake_minimum_required (VERSION 2.8.11)
project (CqlDriverTests)

FILE(GLOB_RECURSE Files ./*.cpp)
add_executable(${PROJECT_NAME} ${Files})

add_subdirectory(../3rd-party/googletest googletest EXCLUDE_FROM_ALL)

include_directories(../include)
target_link_libraries(${PROJECT_NAME} gtest_main)
set(CMAKE_VERBOSE_MAKEFILE TRUE)



