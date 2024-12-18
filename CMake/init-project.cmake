function(InitDefaultProject name)

    target_compile_features(${name} PRIVATE cxx_std_20)

    # set output directory
    set(BINARY_OUT ${PROJECT_SOURCE_DIR}/bin/${name}/${ARCHITECTURE}/$<IF:$<CONFIG:Debug>,Debug,Release>)


    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUT}
        LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUT}
        ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUT}
    )

    if(MSVC)
        target_compile_options(${name} PRIVATE /W4 /WX)
    else()
        target_compile_options(${name} PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()

endfunction()