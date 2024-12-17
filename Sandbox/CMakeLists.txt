cmake_minimum_required(VERSION 3.24)

# dependencies provider
# set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES POWEngine_dependencies_provider.cmake)

# define x86 or x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "Win32")
endif()

# Function to get all .cpp and .h files in a directory
function(get_source_files DIRECTORY OUTPUT_VAR)
    file(GLOB_RECURSE SOURCE_FILES
        "${DIRECTORY}/*.cpp"
        "${DIRECTORY}/*.h"
    )
    set(${OUTPUT_VAR} ${SOURCE_FILES} PARENT_SCOPE)
endfunction()

get_source_files(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

add_executable(Sandbox ${SRCS})

target_compile_features(Sandbox PRIVATE cxx_std_20)

target_include_directories(Sandbox PRIVATE ${PROJECT_SOURCE_DIR}/POWEngine)

# set output directory
set(OUTPUT ${PROJECT_SOURCE_DIR}/bin/Sandbox/${ARCHITECTURE}/$<IF:$<CONFIG:Debug>,Debug,Release>)


set_target_properties(Sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT}
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT}
)

if(MSVC)
    target_compile_options(Sandbox PRIVATE /W4 /WX)
else()
    target_compile_options(Sandbox PRIVATE -Wall -Wextra -pedantic -Werror)
endif()