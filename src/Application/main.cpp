#include "WallpaperEngine/WallpaperEngine.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <cassert>
#include <unistd.h>
#include <Utilities/defines.hpp>
#include <Engine/WinEngine/engine.hpp>
#include <Engine/WinEngine/XManager.hpp>
int main() {
    xppr::log::init_logger();
    
    auto* win = test_open_window();
    
    xppr::wpeng::WallpaperEngine app({win});
    app.loadPlugin("build/src/Plugins/API/Python/libpaperpy.so");
    app.run();   

    win->hide();
    auto manager = winengine::XWindowManager::getInstance();
    manager->closeDisplay();

}
