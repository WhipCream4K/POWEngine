cmake_minimum_required(VERSION 3.24)

# dependencies provider
# set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES dependencies_provider.cmake)

project(POW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(POWEngine)
