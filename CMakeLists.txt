cmake_minimum_required(VERSION 3.10)

project(evsh)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)
endif()
