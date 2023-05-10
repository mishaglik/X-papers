set(WINDOW_ENGINE_HEADERS
    WindowEngine.hpp
    XDisplayHandler.hpp
    XWindowHandler.hpp
    proto.hpp
    engine.hpp
)

set(WINDOW_ENGINE_SOURCES
    engine.cpp
    WindowEngine.cpp
    XDisplayHandler.cpp
    XWindowHandler.cpp
)

add_library(windowEngine ${WINDOW_ENGINE_HEADERS} ${WINDOW_ENGINE_SOURCES})
make_component(windowEngine WindowEngine)
target_include_directories(windowEngine PUBLIC ..)
target_link_libraries(windowEngine PUBLIC sfml-graphics sfml-window sfml-system)
link_directories(/usr/local/lib/)
target_link_libraries(windowEngine PUBLIC sfeMovie)

find_package(XCB REQUIRED)
find_package(X11 REQUIRED)
find_package(X11 REQUIRED COMPONENTS Xrandr)

target_include_directories(windowEngine PUBLIC ${XCB_INCLUDE_DIR} ${X11_INCLUDE_DIR})
target_link_libraries(windowEngine PUBLIC ${XCB_LIBRARIES} ${X11_LIBRARIES} X11::Xrandr)

target_link_libraries(windowEngine PRIVATE utilities)

set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)
