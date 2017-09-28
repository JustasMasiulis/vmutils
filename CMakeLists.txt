cmake_minimum_required(VERSION 3.3)
project(vmi)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories (${PROJECT_SOURCE_DIR}/include)
add_library (vmi INTERFACE)
target_include_directories (vmi INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_sources (vmi INTERFACE ${PROJECT_SOURCE_DIR}/include/vmi.hpp)

enable_testing()
add_subdirectory (test)