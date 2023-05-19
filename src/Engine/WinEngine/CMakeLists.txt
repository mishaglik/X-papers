set(WINDOW_ENGINE_HEADERS
    WindowEngine.hpp
    XDisplayHandler.hpp
    XWindowHandler.hpp
    proto.hpp
    engine.hpp
    VideoHandler.hpp
    glfwVideo.hpp
    glfwRender.hpp

)

set(WINDOW_ENGINE_SOURCES
    engine.cpp
    WindowEngine.cpp
    XDisplayHandler.cpp
    XWindowHandler.cpp
    VideoHandler.cpp
    glfwVideo.cpp
    glfwRender.cpp
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
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavdevice
    libavfilter
    libavformat
    libavcodec
    libswresample
    libswscale
    libavutil
)

target_link_libraries(windowEngine PUBLIC PkgConfig::LIBAV)


target_include_directories(windowEngine PUBLIC ${XCB_INCLUDE_DIR} ${X11_INCLUDE_DIR})
target_link_libraries(windowEngine PUBLIC ${XCB_LIBRARIES} ${X11_LIBRARIES} X11::Xrandr)

target_link_libraries(windowEngine PRIVATE utilities)
target_link_libraries(windowEngine PUBLIC glfw)
target_link_libraries(windowEngine PUBLIC ${OPENGL_LIBRARY})

set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)
