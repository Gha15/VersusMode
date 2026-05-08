cmake_minimum_required(VERSION 3.16)
project(VersusMode)

set(CMAKE_CXX_STANDARD 17)

# Path to Geode SDK (adjust to your actual location)
set(GEODE_SDK_PATH "${CMAKE_SOURCE_DIR}/../Geode")

add_library(VersusMode SHARED main.cpp)

# Include Geode and Globed headers
include_directories(${GEODE_SDK_PATH}/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/globed/include)

# Link against Geode and Globed libraries
target_link_libraries(VersusMode
    ${GEODE_SDK_PATH}/lib/geode.lib
    ${CMAKE_SOURCE_DIR}/libs/globed/lib/globed.lib
)
