function(CollectSources dir out_var)
    file(GLOB entries "${dir}/*")

    foreach(entry ${entries})
        if(IS_DIRECTORY ${entry})
            CollectSources(${entry} files)
            list(APPEND ${out_var} ${files})
        elseif(entry MATCHES "\\.(cpp|h)$")
            list(APPEND ${out_var} ${entry})
        endif()
    endforeach()

    set(${out_var} ${${out_var}} PARENT_SCOPE)
endfunction()