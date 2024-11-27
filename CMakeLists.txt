cmake_minimum_required(VERSION 3.14)

project(POW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# define x86 or x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "Win32")
endif()

# gather source files
include(collect_sources.cmake)
GetPOWSources(SRCS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/POWEngine ${CMAKE_CURRENT_SOURCE_DIR}/POWEngine/PCH)

# set output directory
set(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PROJECT_NAME}/${ARCHITECTURE}/${CMAKE_BUILD_TYPE})

add_library(${PROJECT_NAME} STATIC ${SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT}
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/POWEngine/PCH/pch.h
)

# dependencies
include(dependencies.cmake)