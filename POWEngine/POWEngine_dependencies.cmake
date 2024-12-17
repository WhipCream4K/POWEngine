
include(FetchContent)

# glm is header only library
set(GLM_DIR ${PROJECT_SOURCE_DIR}/vendor/glm)
target_include_directories(POWEngine PRIVATE ${GLM_DIR})

# glfw
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/vendor/glfw)

# find_package(glfw3 HINTS ${GLFW_DIR}/bin QUIET)
FetchContent_Declare(
    glfw3
    SOURCE_DIR ${GLFW_DIR}
    BINARY_DIR ${GLFW_DIR}/build
    FIND_PACKAGE_ARGS HINTS ${GLFW_DIR}/bin QUIET
)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_INSTALL OFF)

FetchContent_MakeAvailable(glfw3)

target_include_directories(POWEngine PRIVATE ${GLFW_DIR}/include)
target_link_libraries(POWEngine glfw3)

# glad
set(GLAD_DIR ${PROJECT_SOURCE_DIR}/vendor/glad)

FetchContent_Declare(
    glad
    SOURCE_DIR ${GLAD_DIR}
    BINARY_DIR ${GLAD_DIR}/build
)

FetchContent_MakeAvailable(glad)

target_include_directories(POWEngine PRIVATE ${GLAD_DIR}/include)
target_link_libraries(POWEngine glad::glad)