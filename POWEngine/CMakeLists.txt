cmake_minimum_required(VERSION 3.24)

set(ENGINE_NAME POWEngine)

# dependencies provider
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES POWEngine_dependencies_provider.cmake)

# define x86 or x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "Win32")
endif()

# gather source files
include(collect_sources.cmake)
GetPOWSources(SRCS)

add_library(${ENGINE_NAME} STATIC ${SRCS})

target_compile_features(${ENGINE_NAME} PRIVATE cxx_std_20)

target_include_directories(${ENGINE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/PCH)

# set output directory
set(OUTPUT ${PROJECT_SOURCE_DIR}/bin/${ENGINE_NAME}/${ARCHITECTURE}/$<IF:$<CONFIG:Debug>,Debug,Release>)


set_target_properties(${ENGINE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT}
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT}
)

if(MSVC)
    target_compile_options(${ENGINE_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${ENGINE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_precompile_headers(${ENGINE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/PCH/pch.h
)

include(POWEngine_dependencies.cmake)