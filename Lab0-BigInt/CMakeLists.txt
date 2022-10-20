cmake_minimum_required(VERSION 3.23)
project(xf_Lab1_BigInt_ver2)

add_subdirectory(googletest)

include_directories(googletest/include)

add_executable(tests UnitTests.cpp BigInt.cpp)

target_link_libraries(tests PRIVATE gtest)

set(CMAKE_CXX_STANDARD 17)

add_executable(xf_Lab1_BigInt_ver2 main.cpp BigInt.h BigInt.cpp)
