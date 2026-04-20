cmake_minimum_required(VERSION 3.13.4)

# This name should match your folder/repo name
project(VersusMode)

find_package(Geode REQUIRED)

# We use the PROJECT_NAME variable to avoid typos
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES 
        src/main.cpp
)

# This links your mod.json and resources
setup_geode_mod(${PROJECT_NAME})
