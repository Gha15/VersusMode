cmake_minimum_required(VERSION 3.13.4)

project(gha16.versusmode)

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES 
        src/main.cpp
)

setup_geode_mod(${PROJECT_NAME})
