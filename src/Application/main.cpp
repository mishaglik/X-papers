#include <unistd.h>
#include <Engine/WinEngine/engine.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <Utilities/defines.hpp>
#include <Engine/WinEngine/engine.hpp>
#include "Configurator.hpp"
#include "WallpaperEngine/WallpaperEngine.hpp"
#include "WinEngine/XDisplayHandler.hpp"

int main(int argc, const char* argv[]) {
    xppr::log::init_logger();
    
//     test_video_handler("/home/execorn/Downloads/wallpaper.mp4");
  
    auto* win = test_open_window();
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
