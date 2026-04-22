cmake_minimum_required(VERSION 3.13.4)

# 1. This must come BEFORE find_package
project(VersusMode)

# 2. Helpful hint for Mac/Android runners
if(NOT Geode_DIR AND DEFINED ENV{GEODE_SDK})
    set(Geode_DIR "$ENV{GEODE_SDK}")
endif()

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES 
        main.cpp
)

setup_geode_mod(${PROJECT_NAME})
