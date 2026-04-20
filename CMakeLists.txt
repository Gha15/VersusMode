cmake_minimum_required(VERSION 3.13.4)

project(VersusMode)

# Find the Geode SDK
find_package(Geode REQUIRED)

# Define the mod and its source files
add_geode_mod(
    gha16.versusmode
    VERSION v1.0.0
    SOURCES 
        src/main.cpp
)

# This is the magic line for Globed/Node-IDs to work on GitHub Actions
setup_geode_mod(gha16.versusmode)