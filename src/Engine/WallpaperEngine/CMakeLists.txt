set(WP_ENGINE_HEADERS
    Background.hpp
    Event.hpp
    Widget.hpp
    WallpaperEngine.hpp
)

set(WP_ENGINE_SOURCES
    Background.cpp
    Event.cpp
    Widget.cpp
    WallpaperEngine.cpp
)

add_library(wallpaperEngine ${WP_ENGINE_HEADERS} ${WP_ENGINE_SOURCES})
make_component(wallpaperEngine WallpaperEngine)
target_include_directories(wallpaperEngine PUBLIC ..)

target_link_libraries(wallpaperEngine PRIVATE utilities renderEngine windowEngine)
set (COMPONENT_LIST ${COMPONENT_LIST} PARENT_SCOPE)