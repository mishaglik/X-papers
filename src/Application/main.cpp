#include <unistd.h>

#include <Engine/WinEngine/engine.hpp>
#include <WallpaperEngine/WallpaperEngine.hpp>
#include <Utilities/utils.hpp>

#include "Configurator.hpp"

int main(int argc, const char* argv[]) {
    xppr::log::init_logger();
    Configurator cfg = Configurator::fromArguments(argc, argv);

    if (cfg.isDemon()) {
        daemonize();
    }

    auto* win = openSimpleWindow(nullptr, cfg.isVertSyncEnabled(), cfg.getFramerateLimit());

    xppr::wpeng::WallpaperEngine app({win});

    setupSignals(&app);

    app.loadPlugin("build/src/Plugins/API/Python/libpaperpy.so");
    app.run();

    auto manager = winengine::XDisplayHandler::getInstance();
    manager->closeDisplay();

    xppr::log::info("XPapers exited gracefully!");
    return 0;
}
