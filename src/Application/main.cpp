#include "WallpaperEngine/WallpaperEngine.hpp"
#include "WinEngine/XDisplayHandler.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <cassert>
#include <unistd.h>
#include <Utilities/defines.hpp>
#include <Engine/WinEngine/engine.hpp>

int main() {
    xppr::log::init_logger();
    

    
//     test_video_handler("/home/execorn/Downloads/wallpaper.mp4");
  
    auto* win = test_open_window();
    
    xppr::wpeng::WallpaperEngine app({win});
    app.loadPlugin("build/src/Plugins/API/Python/libpaperpy.so");
    app.run();   
    auto manager = winengine::XDisplayHandler::getInstance();
    win->hide();
    
    manager->closeDisplay();
}
