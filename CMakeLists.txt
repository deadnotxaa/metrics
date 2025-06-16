cmake_minimum_required(VERSION 3.12)

project(
        metrics
        VERSION 1.0
        DESCRIPTION "C++ Library for collecting metrics"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

include(cmake/ExternalLibraries.cmake)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
