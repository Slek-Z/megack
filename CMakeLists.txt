project(megack CXX)

cmake_minimum_required(VERSION 2.8)

## Set CXX optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpedantic -Wall -Wextra")

## Set build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")
message(STATUS "Flags: " ${CMAKE_CXX_FLAGS})

## Dependencies
set(MEGA_INSTALL_PREFIX "/" CACHE PATH "Mega SDK install prefix")
include_directories("${MEGA_INSTALL_PREFIX}/include/" "${MEGA_INSTALL_PREFIX}/include/mega/posix/")
link_directories("${MEGA_INSTALL_PREFIX}/lib/")

## Link executable
add_executable(megack megack.cpp)
target_link_libraries(megack mega)
