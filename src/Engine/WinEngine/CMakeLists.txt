set(WINDOW_ENGINE_HEADERS
)

set(WINDOW_ENGINE_SOURCES
    engine.cpp
)

add_library(windowEngine ${WINDOW_ENGINE_HEADERS} ${WINDOW_ENGINE_SOURCES})
make_component(windowEngine WindowEngine)
target_include_directories(windowEngine PUBLIC ..)

target_link_libraries(windowEngine utilities)
set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)