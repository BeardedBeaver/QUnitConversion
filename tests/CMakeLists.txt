cmake_minimum_required(VERSION 3.19)

include_guard(GLOBAL)

project(QUnitConversionTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

option(QUNITCONV_QT_TESTS "Include Qt string types in typed tests" OFF)

set(SOURCES
    qaliasdictionary_tests.cpp
    qlinearfunction_tests.cpp
    qunitconvertor_tests.cpp
)

add_executable(QUnitConversionTests
    ${SOURCES}
)

target_link_libraries(QUnitConversionTests
    GTest::gtest_main
    QUnitConversion
)

if(QUNITCONV_QT_TESTS)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
    target_link_libraries(QUnitConversionTests Qt::Core)
    target_compile_definitions(QUnitConversionTests PRIVATE QUNITCONV_QT_TESTS)
endif()

include(GoogleTest)
gtest_discover_tests(QUnitConversionTests)
