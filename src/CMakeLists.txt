cmake_minimum_required(VERSION 3.19)

include_guard(GLOBAL)

project(QUnitConversion LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(SOURCES
    qlinearfunction.cpp
)

set(HEADERS
    qaliasdictionary.h
    qlinearfunction.h
    qunitconversionfamily.h
    qunitconversionrule.h
    qunitconvertor.h
)

add_library(QUnitConversion STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(QUnitConversion PUBLIC
    .
)

