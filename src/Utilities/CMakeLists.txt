
SET(UTILS_HEADERS
    defines.hpp
    log.hpp
)

SET(UTILS_SOURCES
    log.cpp
)


add_library(utilities_headers INTERFACE ${UTILS_HEADERS})
target_include_directories(utilities_headers INTERFACE ..)
target_include_directories(utilities_headers SYSTEM INTERFACE $<TARGET_PROPERTY:spdlog,INTERFACE_INCLUDE_DIRECTORIES>)

add_library(utilities ${UTILS_HEADERS} ${UTILS_SOURCES})
make_component(utilities Utils)
target_include_directories(utilities PUBLIC ..)
target_link_libraries(utilities PUBLIC spdlog)

set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)