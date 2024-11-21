# cmake/dependencies.cmake
include(FetchContent)


# glm
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_DIR})

