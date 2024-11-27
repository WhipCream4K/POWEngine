# cmake/dependencies.cmake
include(FetchContent)


# glm
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_DIR})

# glfw
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)
set(GLFW_OPTIONS
    -DBUILD_SHARED_LIBS=OFF
    -DGLFW_BUILD_EXAMPLES=OFF
    -DGLFW_BUILD_TESTS=OFF
    -DGLFW_BUILD_DOCS=OFF
    -DCMAKE_INSTALL_PREFIX=${GLFW_DIR}/glfw
)

FetchContent_Declare(
    glfw
    PREFIX ${GLFW_DIR}
    SOURCE_DIR ${GLFW_DIR}
    CMAKE_ARGS ${GLFW_OPTIONS}
    FIND_PACKAGES_ARGS NAMES glfw3
)

FetchContent_MakeAvailable(glfw)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw opengl32)