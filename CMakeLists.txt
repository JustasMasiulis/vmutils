cmake_minimum_required(VERSION 3.3)
project(vmu)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(detail_header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vmu/detail/windows/query.inl
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vmu/detail/query_range_using_query.inl)

set(header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vmu.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vmu/protection.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vmu/query.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vmu/region.hpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(vmu INTERFACE)
target_sources(vmu INTERFACE $<BUILD_INTERFACE:${detail_header_files} ${header_files}>)
target_include_directories(vmu INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(vmu SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

enable_testing()
add_subdirectory(test)