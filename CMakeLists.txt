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

# adding sources files 
function(CollectSources dir out_var)
    file(GLOB entries "${dir}/*")

    foreach(entry ${entries})
        if(IS_DIRECTORY ${entry})
            CollectSources(${entry} files)
            list(APPEND ${out_var} ${files})
        elseif(entry MATCHES "\\.(cpp)$")
            list(APPEND ${out_var} ${entry})
        endif()
    endforeach()

    set(${out_var} ${${out_var}} PARENT_SCOPE)
endfunction()

CollectSources(${CMAKE_CURRENT_SOURCE_DIR}/POWEngine SRCS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/POWEngine ${CMAKE_CURRENT_SOURCE_DIR}/POWEngine/PCH)

# set output directory
set(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}/${ARCHITECTURE})

add_library(${PROJECT_NAME} STATIC ${SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT}
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/POWEngine/PCH/pch.h
)






