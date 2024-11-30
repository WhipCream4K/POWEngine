

# glm is header only library
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)
target_include_directories(${ENGINE_NAME} PRIVATE ${GLM_DIR})

# glfw
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)

find_package(glfw3 CONFIG HINTS ${GLFW_DIR}/bin/lib QUIET)
target_include_directories(${ENGINE_NAME} PRIVATE ${GLFW_DIR}/bin/include)
target_link_libraries(${ENGINE_NAME} glfw3)

# glad
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)
target_include_directories(${ENGINE_NAME} PRIVATE ${GLAD_DIR}/include)