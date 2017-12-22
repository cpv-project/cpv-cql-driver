cmake_minimum_required (VERSION 2.8.11)
project (CqlDriver)

FILE(GLOB_RECURSE Files ./*.cpp)
add_library(${PROJECT_NAME} STATIC ${Files})

include_directories(../include)
target_link_libraries(${PROJECT_NAME})
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g --std=c++14")

