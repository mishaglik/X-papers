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

    // xppr::Vector<xppr::Image> bg(2);
    // bg[0].loadFromFile("/home/gms/I1.jpg");
    // bg[1].loadFromFile("/home/gms/I2.jpg");
    xppr::WallpaperEngine app({win});
    // app.setBackgroundImages(bg, 0);
    app.run();   

    win->hide();
    auto manager = winengine::XDisplayHandler::getInstance();
    manager->closeDisplay();
}
