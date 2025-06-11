cmake_minimum_required(VERSION 3.20)

project(Mathub LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Since Mathub is header-only, we just need to set the include directory
add_library(mathub INTERFACE)
target_include_directories(mathub INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Executable to run the example
add_executable(Mathub example.cpp)
target_link_libraries(Mathub PRIVATE mathub)