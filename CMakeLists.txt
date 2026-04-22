cmake_minimum_required(VERSION 3.13.4)

# 1. Name your project (matches your mod.json ID)
project(gha16.versusmode)

# 2. Link the Geode SDK
find_package(Geode REQUIRED)

# 3. Define the build process
# We are looking for main.cpp in the ROOT folder (no src/ folder)
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES 
        main.cpp
)

# 4. Finalize the mod file (.geode)
setup_geode_mod(${PROJECT_NAME})
