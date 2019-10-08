cmake_minimum_required(VERSION 3.10)

project(sh)

set(CMAKE_CXX_STANDARD 17)

file(GLOB sh_SRC "*.h" "*.cpp")

add_executable(sh ${sh_SRC})