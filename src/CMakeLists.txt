set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES commandline.cpp commandregistry.cpp exec.cpp parse.cpp)

add_library(${BINARY}_lib STATIC ${SOURCES})
target_include_directories(${BINARY}_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(${BINARY} main.cpp)
target_link_libraries(${BINARY} ${BINARY}_lib)
