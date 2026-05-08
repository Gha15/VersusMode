cmake_minimum_required(VERSION 3.16)
project(VersusMode)

set(CMAKE_CXX_STANDARD 17)

# Path to Geode SDK (adjust this to your actual install)
set(GEODE_SDK_PATH "${CMAKE_SOURCE_DIR}/../Geode")

add_library(VersusMode SHARED main.cpp)

# Include Geode and Globed headers
include_directories(${GEODE_SDK_PATH}/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/globed/include)

# Link against Geode and Globed libraries
if (WIN32)
    target_link_libraries(VersusMode
        ${GEODE_SDK_PATH}/lib/geode.lib
        ${CMAKE_SOURCE_DIR}/libs/globed/lib/globed.lib
    )
elseif(APPLE)
    target_link_libraries(VersusMode
        ${GEODE_SDK_PATH}/lib/libgeode.dylib
        ${CMAKE_SOURCE_DIR}/libs/globed/lib/libglobed.dylib
    )
elseif(ANDROID)
    target_link_libraries(VersusMode
        ${GEODE_SDK_PATH}/lib/libgeode.so
        ${CMAKE_SOURCE_DIR}/libs/globed/lib/libglobed.so
    )
endif()
