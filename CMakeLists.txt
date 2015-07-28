cmake_minimum_required(VERSION 2.8.8)

project(little-big-int)

add_definitions(${CMAKE_CXX_FLAGS} "-std=c++11")
add_definitions(${CMAKE_CXX_FLAGS} "-Wall")
add_definitions(${CMAKE_CXX_FLAGS} "-pedantic")
add_definitions(${CMAKE_CXX_FLAGS} "-fexceptions")
add_definitions(${CMAKE_CXX_FLAGS} "-fPIC")

install(FILES src/little-big-int.hpp DESTINATION include/little-big-int)
add_library(littlebigint SHARED src/little-big-int.cpp)

enable_testing()
add_subdirectory(third/gtest-1.7.0)

include_directories(src third/gtest-1.7.0/include third/gtest-1.7.0)
add_subdirectory(test)

