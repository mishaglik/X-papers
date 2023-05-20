set(RENDER_ENGINE_HEADERS
)

set(RENDER_ENGINE_SOURCES
    REngine.cpp
)

add_library(renderEngine ${RENDER_ENGINE_HEADERS} ${RENDER_ENGINE_SOURCES})
make_component(renderEngine RenderEngine)
target_include_directories(renderEngine PUBLIC ..)

target_link_libraries(renderEngine PUBLIC sfml-graphics sfml-window sfml-system)

target_link_libraries(renderEngine PUBLIC utilities)
set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)