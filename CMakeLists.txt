cmake_minimum_required(VERSION 3.13.4)
project(VersusMode)

find_package(Geode REQUIRED)

add_geode_mod(
    VersusMode
    SOURCES main.cpp
)

setup_geode_mod(VersusMode)
