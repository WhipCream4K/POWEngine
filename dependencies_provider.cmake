cmake_minimum_required(VERSION 3.24)



macro(my_dependencies_provider method package_name)

    # glfw
    set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)

    set(GLFW_OPTIONS 
        -DBUILD_SHARED_LIBS=OFF
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
        -DGLFW_INSTALL=ON
        -DCMAKE_INSTALL_PREFIX=${GLFW_DIR}/bin
    )

    if(NOT EXISTS ${GLFW_DIR}/bin/lib)
        execute_process(
            COMMAND ${CMAKE_COMMAND} ${GLFW_OPTIONS} -S ${GLFW_DIR} -B ${GLFW_DIR}/build -G ${CMAKE_GENERATOR}
            COMMAND ${CMAKE_COMMAND} --build ${GLFW_DIR}/build --config ${CMAKE_BUILD_TYPE} --target install
        )
    endif()


endmacro()

cmake_language(
    SET_DEPENDENCY_PROVIDER my_dependencies_provider
    SUPPORTED_METHODS FIND_PACKAGE
)
