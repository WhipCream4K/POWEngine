# Collect Sources file for POWEngine

set(POW_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

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

function (GetPOWSources out_var)

    # Core
    CollectSources(${POW_SOURCES_DIR}/Core POW_SOURCES)

    # ECS
    CollectSources(${POW_SOURCES_DIR}/ECS POW_SOURCES)

    # Game
    CollectSources(${POW_SOURCES_DIR}/Game POW_SOURCES)

    # Lock Free
    CollectSources(${POW_SOURCES_DIR}/LockFree POW_SOURCES)

    # Platform
    CollectSources(${POW_SOURCES_DIR}/Platform POW_SOURCES)

    # Random
    CollectSources(${POW_SOURCES_DIR}/Random POW_SOURCES)

    # Renderer
    CollectSources(${POW_SOURCES_DIR}/Renderer POW_SOURCES)

    # Sound
    CollectSources(${POW_SOURCES_DIR}/Sound POW_SOURCES)

    # Utils
    CollectSources(${POW_SOURCES_DIR}/Utils POW_SOURCES)

    set(${out_var} ${POW_SOURCES} PARENT_SCOPE)

endfunction()