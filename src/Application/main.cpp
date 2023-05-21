#include <unistd.h>
#include <Engine/WinEngine/engine.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <Utilities/defines.hpp>
#include <cassert>
#include "WallpaperEngine/WallpaperEngine.hpp"
#include "WinEngine/XDisplayHandler.hpp"

int main() {
    xppr::log::init_logger();
    /*
    auto* win = openSimpleWindow(nullptr);
    auto manager = winengine::XDisplayHandler::getInstance();
    
    xppr::wpeng::WallpaperEngine app({win});

    app.loadPlugin("build/src/Plugins/API/Python/libpaperpy.so");
    app.run();   

    win->hide();
    
    manager->closeDisplay();
    */
    test_video_handler("/home/execorn/Downloads/wallpaper.mp4");

    return 0;
}
