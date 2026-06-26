cmake_minimum_required(VERSION 3.14)
project(range_finders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only library
add_library(range_finders INTERFACE)

target_include_directories(range_finders INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# spatial_ds is required (provides OctTree, KDTree, RangeTree, GroupRangeTree)
if(TARGET spatial_ds)
    target_link_libraries(range_finders INTERFACE spatial_ds)
endif()
