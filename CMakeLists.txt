cmake_minimum_required(VERSION 3.16)

project(scoped-pointer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)

add_executable(scoped-pointer-test
  ScopedPointer.h ScopedPointerTest.cpp)

target_link_libraries(scoped-pointer-test
 PRIVATE
  GTest::GTest)

add_test(scoped-pointer-gtest scoped-pointer-test)
