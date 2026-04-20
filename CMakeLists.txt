cmake_minimum_required(VERSION 3.13.4)

# Use a simple project name to avoid linking errors
project(VersusMode)

find_package(Geode REQUIRED)

# This tells the compiler the file is right here, not in a folder
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES 
        main.cpp
)

setup_geode_mod(${PROJECT_NAME})
