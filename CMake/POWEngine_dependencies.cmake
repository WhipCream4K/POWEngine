
include(FetchContent)

# glm is header only library
set(GLM_DIR ${PROJECT_SOURCE_DIR}/vendor/glm)
target_include_directories(POWEngine PRIVATE ${GLM_DIR})

# glfw
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/vendor/glfw)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    SOURCE_DIR ${GLFW_DIR}
    BINARY_DIR ${GLFW_DIR}/build
    FIND_PACKAGE_ARGS HINTS ${GLFW_DIR}/bin QUIET
)

FetchContent_MakeAvailable(glfw)

target_include_directories(POWEngine PRIVATE ${GLFW_DIR}/include)
target_link_libraries(POWEngine INTERFACE glfw)

# glad
set(GLAD_DIR ${PROJECT_SOURCE_DIR}/vendor/glad)

FetchContent_Declare(
    glad
    SOURCE_DIR ${GLAD_DIR}
    BINARY_DIR ${GLAD_DIR}/build
)

FetchContent_MakeAvailable(glad)

target_include_directories(POWEngine PRIVATE ${GLAD_DIR}/include)
target_link_libraries(POWEngine PRIVATE glad::glad)