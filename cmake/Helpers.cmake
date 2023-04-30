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

function(make_component COMPONENT COMPONENT_NAME)
    list(APPEND COMPONENT_LIST "${COMPONENT_NAME}")
    list(REMOVE_DUPLICATES COMPONENT_LIST)
    set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)
    set (COMPONENT_LIST ${COMPONENT_LIST} CACHE INTERNAL "components")
    project_log("Add component: ${COMPONENT_NAME}")
    target_compile_definitions(${COMPONENT} PRIVATE XPPR_COMPONENT=\"${COMPONENT_NAME}\")
endfunction()

function(prepare_executable EXECUTABLE EXECUTABLE_NAME)
    make_component(${EXECUTABLE} ${EXECUTABLE_NAME})
    list(TRANSFORM COMPONENT_LIST PREPEND "\"")
    list(TRANSFORM COMPONENT_LIST APPEND "\"")
    list(JOIN COMPONENT_LIST ", " COMPONENTS)
    project_log("Built components: ${COMPONENT_LIST}")
    target_compile_definitions(${EXECUTABLE} PRIVATE XPPR_COMPONENTS=${COMPONENTS})
endfunction()