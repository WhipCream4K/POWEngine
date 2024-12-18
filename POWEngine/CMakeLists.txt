cmake_minimum_required(VERSION 3.24)

# define x86 or x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "Win32")
endif()

include(../CMake/collect_sources.cmake)
include(../CMake/init-project.cmake)

set(POWE_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(POWE_SRCS "")

# Core
CollectSources(${POWE_SOURCES_DIR}/Core POWE_SRCS)
# Logger
CollectSources(${POWE_SOURCES_DIR}/Logger POWE_SRCS)
# ECS
CollectSources(${POWE_SOURCES_DIR}/ECS POWE_SRCS)
# Game
CollectSources(${POWE_SOURCES_DIR}/Game POWE_SRCS)
# Lock Free
CollectSources(${POWE_SOURCES_DIR}/LockFree POWE_SRCS)
# Platform
CollectSources(${POWE_SOURCES_DIR}/Platform POWE_SRCS)
# Random
CollectSources(${POWE_SOURCES_DIR}/Random POWE_SRCS)
# Renderer
CollectSources(${POWE_SOURCES_DIR}/Renderer POWE_SRCS)
# Sound
CollectSources(${POWE_SOURCES_DIR}/Sound POWE_SRCS)
# Utils
CollectSources(${POWE_SOURCES_DIR}/Utils POWE_SRCS)


add_library(POWEngine STATIC ${POWE_SRCS})

InitDefaultProject(POWEngine)

target_include_directories(POWEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/PCH
    ${CMAKE_CURRENT_SOURCE_DIR})

target_precompile_headers(POWEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/PCH/pch.h
)

include(../CMake/POWEngine_dependencies.cmake)