cmake_minimum_required(VERSION 3.24)

function(BuildDependencies Dir Options)

    if(GENERATOR_IS_MULTI_CONFIG)
        set(CONFIG_ARGS "-DCMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES}")
    else()
        set(CONFIG_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
    endif()

    if(NOT EXISTS ${Dir}/bin)
        execute_process(
            COMMAND ${CMAKE_COMMAND} ${Options} ${CONFIG_ARGS} -S ${Dir} -B ${Dir}/build -G ${CMAKE_GENERATOR}
            RESULT_VARIABLE result
        )

        if(result EQUAL 0)

            if(GENERATOR_IS_MULTI_CONFIG)
                execute_process(
                    COMMAND ${CMAKE_COMMAND} --build ${Dir}/build --config Release
                    COMMAND ${CMAKE_COMMAND} --build ${Dir}/build --config Debug
                    RESULT_VARIABLE result
                )
            else()
                execute_process(
                    COMMAND ${CMAKE_COMMAND} --build ${Dir}/build
                    RESULT_VARIABLE result
                )
            endif()

        endif()

        if(result EQUAL 0)

            if(GENERATOR_IS_MULTI_CONFIG)
                execute_process(
                    COMMAND ${CMAKE_COMMAND} --install ${Dir}/build --prefix ${Dir}/bin/Release --config Release
                    COMMAND ${CMAKE_COMMAND} --install ${Dir}/build --prefix ${Dir}/bin/Debug --config Debug
                )

            else()
                execute_process(
                    COMMAND ${CMAKE_COMMAND} --install ${Dir}/build --prefix ${Dir}/bin/${CMAKE_BUILD_TYPE}
                )
            endif()


        endif()

        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Failed to build ${Dir}")
        endif()

    endif()

endfunction() 

# GLFW
function(Build_GLFW)
    
    set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)

    set(GLFW_OPTIONS 
        -DBUILD_SHARED_LIBS=OFF
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
        -DGLFW_INSTALL=ON
    )

    BuildDependencies(${GLFW_DIR} "${GLFW_OPTIONS}")

endfunction()

macro(my_dependencies_provider method package_name)
    Build_GLFW()
endmacro()

cmake_language(
    SET_DEPENDENCY_PROVIDER my_dependencies_provider
    SUPPORTED_METHODS FIND_PACKAGE
)
