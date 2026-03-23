cmake_minimum_required(VERSION 3.19)
project(QUnitConversion)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

option(QUNITCONV_BUILD_TESTS "Build QUnitConversion unttests" On)

if (QUNITCONV_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
