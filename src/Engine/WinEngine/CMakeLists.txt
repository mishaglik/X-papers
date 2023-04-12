set(WINDOW_ENGINE_HEADERS
    engine.hpp
)

set(WINDOW_ENGINE_SOURCES
    engine.cpp
)

add_library(windowEngine ${WINDOW_ENGINE_HEADERS} ${WINDOW_ENGINE_SOURCES})
make_component(windowEngine WindowEngine)
target_include_directories(windowEngine PUBLIC ..)

find_package(X11 REQUIRED)

project_log("X11_FOUND = ${X11_FOUND}")
project_log("X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
project_log("X11_LIBRARIES = ${X11_LIBRARIES}")

target_include_directories(windowEngine PRIVATE ${X11_INCLUDE_DIR})
target_link_libraries(windowEngine PRIVATE ${X11_LIBRARIES} utilities)

set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)