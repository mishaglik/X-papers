#include "WallpaperEngine/WallpaperEngine.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <cassert>
#include <unistd.h>
#include <Utilities/defines.hpp>
#include <SFML/Window.hpp>
int main() {
    xppr::log::init_logger();
    
    sf::Window window(sf::VideoMode(800, 600), "Test window");

    xppr::Vector<xppr::Image> bg(2);
    bg[0].loadFromFile("/home/gms/I1.jpg");
    bg[1].loadFromFile("/home/gms/I2.jpg");
    xppr::WallpaperEngine app({window.getSystemHandle()});
    app.setBackgroundImages(bg, 0);
    app.run();   
}
