cmake_minimum_required(VERSION 3.10)

project(evsh)

set(CMAKE_CXX_STANDARD 17)

file(GLOB evsh_SRC "*.h" "*.cpp")

add_executable(evsh ${evsh_SRC})