cmake_minimum_required(VERSION 3.24)

# dependencies provider
# set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES POWEngine_dependencies_provider.cmake)

# define x86 or x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "Win32")
endif()

# gather source files
include(collect_sources.cmake)
GetPOWSources(SRCS)

add_library(POWEngine STATIC ${SRCS})

target_compile_features(POWEngine PRIVATE cxx_std_20)

target_include_directories(POWEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/PCH)

# set output directory
set(OUTPUT ${PROJECT_SOURCE_DIR}/bin/POWEngine/${ARCHITECTURE}/$<IF:$<CONFIG:Debug>,Debug,Release>)


set_target_properties(POWEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT}
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT}
)

if(MSVC)
    target_compile_options(POWEngine PRIVATE /W4 /WX)
else()
    target_compile_options(POWEngine PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_precompile_headers(POWEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/PCH/pch.h
)

include(POWEngine_dependencies.cmake)