
# glm is header only library
set(GLM_DIR ${PROJECT_SOURCE_DIR}/vendor/glm)
target_include_directories(POWEngine PRIVATE ${GLM_DIR})

# glfw
set(GLFW_DIR ${PROJECT_SOURCE_DIR}/vendor/glfw)

find_package(glfw3 HINTS ${GLFW_DIR}/bin QUIET)
target_include_directories(POWEngine PRIVATE ${GLFW_DIR}/include)
target_link_libraries(POWEngine glfw3)

# glad
set(GLAD_DIR ${PROJECT_SOURCE_DIR}/vendor/glad)
target_include_directories(POWEngine PRIVATE ${GLAD_DIR}/include)