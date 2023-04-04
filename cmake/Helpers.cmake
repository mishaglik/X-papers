# Helper functions

# Prepend ${PREFIX} to all arguments and put them in ${VAR}
# Usage: prepend(FILES "${DIR}/" ${NAMES})

function(prepend VAR PREFIX)
    set(LIST_VAR "")
    foreach(ELEM ${ARGN})
        list(APPEND LIST_VAR "${PREFIX}${ELEM}")
    endforeach()
    set(${VAR} "${LIST_VAR}" PARENT_SCOPE)
endfunction()
