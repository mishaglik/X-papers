#include <unistd.h>
#include <Engine/WinEngine/engine.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <Utilities/defines.hpp>
#include "Configurator.hpp"
#include "WallpaperEngine/WallpaperEngine.hpp"
#include "WinEngine/XDisplayHandler.hpp"

int main(int argc, const char* argv[]) {
    xppr::log::init_logger();
    Configurator cfg = Configurator::fromArguments(argc, argv);

    if(cfg.isDemon()) {
        daemonize();
    }

    auto* win = openSimpleWindow(nullptr);

    xppr::wpeng::WallpaperEngine app({win});

    setupSignals(&app);

    app.loadPlugin("build/src/Plugins/API/Python/libpaperpy.so");
    app.run();

    win->hide();

    auto manager = winengine::XDisplayHandler::getInstance();
    manager->closeDisplay();
  
    xppr::log::info("XPapers exited gracefully!");
    return 0;
}
