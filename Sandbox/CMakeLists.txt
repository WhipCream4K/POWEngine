cmake_minimum_required(VERSION 3.24)

# define x86 or x64 architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE "x64")
else()
    set(ARCHITECTURE "Win32")
endif()

include(../CMake/collect_sources.cmake)
include(../CMake/init-project.cmake)

set(SANDBOX_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SANDBOX_SRCS "")

CollectSources(${SANDBOX_SOURCES_DIR} SANDBOX_SRCS)

add_executable(Sandbox ${SANDBOX_SRCS})
InitDefaultProject(Sandbox)

add_dependencies(Sandbox POWEngine)

target_include_directories(Sandbox PRIVATE 
    ${PROJECT_SOURCE_DIR}/POWEngine
)

target_link_libraries(Sandbox POWEngine)