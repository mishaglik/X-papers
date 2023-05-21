////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "engine.hpp"
#include <X11/Xlib.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include "Utilities/log.hpp"
#include "WinEngine/WindowEngine.hpp"
#include "WinEngine/XDisplayHandler.hpp"
#include "WinEngine/proto.hpp"

void test_gl(const char* filename) {
    winengine::GlfwController controller;

    if (!controller.Render(filename)) {
        xppr::log::error("Error rendering video %100s\n", filename);
    }
}

static void handle_events(winengine::VideoHandler* handler, sf::Event& ev) {
    auto video_render_obj = handler->getRenderer();
    if (ev.type == sf::Event::Closed || (ev.type == sf::Event::KeyPressed &&
                                         ev.key.code == sf::Keyboard::Escape)) {
        handler->close();
    }

    if (ev.type == sf::Event::Resized) {
        video_render_obj.fit(
            0, 0, static_cast<float>(handler->getRenderWindow()->getSize().x),
            static_cast<float>(handler->getRenderWindow()->getSize().y));
        handler->setView(sf::View(
            sf::FloatRect(0, 0, static_cast<float>(handler->getSize().x),
                          static_cast<float>(handler->getSize().y))));
    }
}

void test_open_window() {
    auto engine = winengine::WindowEngine::getInstance();
    for (std::size_t i = 0; i < engine.getTotalDisplays(); ++i) {
        std::cout << i << ' ' << engine[i].getScreenCount() << std::endl;
    }

    std::cout << engine.getUnixDirectory() << std::endl;

    auto randrinfo = engine.getRandRInfo();

    for (std::size_t i = 0; i < randrinfo.getMonitorsCount(); ++i) {
        std::cout << i << ' ' << randrinfo[i].getName() << ' '
                  << randrinfo[i].getIndex() << std::endl;
    }

    winengine::win_attr_t attrib;
    attrib.override_redirect = True;

    auto manager = winengine::XDisplayHandler::getInstance();
    auto win = manager->addWindow({0, 0}, {1920, 1080}, 0, winengine::Parent,
                                  CopyFromParent, 0, &attrib);

    auto wintype = manager->createAtom("_NET_WM_WINDOW_TYPE", False);
    auto desktop = manager->createAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);

    win->changeProperty(wintype, XA_ATOM, 32, PropModeReplace,
                        reinterpret_cast<unsigned char*>(&desktop), 1);

    unsigned int ints[2];
    ints[0] = 0xFFFFFFFF;
    ints[1] = 2;

    auto wm_desktop = manager->createAtom("_NET_WM_DESKTOP", True);

    win->changeProperty(wm_desktop, XA_ATOM, 32, PropModeReplace,
                        reinterpret_cast<unsigned char*>(ints), 2);

    // auto wm_state = manager->createAtom("_NET_WM_STATE", true);
    // auto wm_fullscreen = manager->createAtom("_NET_WM_STATE_FULLSCREEN",
    // true);

    // win->changeProperty(wm_state, XA_ATOM, 32, PropModeReplace,
    //                     (unsigned char*)&wm_fullscreen, 1);

    // auto wm_state = manager->createAtom("_NET_WM_STATE", true);
    // auto wm_fullscreen = manager->createAtom("_NET_WM_STATE_FULLSCREEN",
    // true);

    // win->changeProperty(wm_state, XA_ATOM, 32, PropModeReplace,
    //                     (unsigned char*)&wm_fullscreen, 1);

    win->setBackground(0xFFFFFF);

    win->clear();
    win->show();

    auto disp = manager->getDisplay();
    while (true) {
        XEvent event;
        XNextEvent(disp, &event);
    }

    win->hide();
    manager->closeDisplay();
}

sf::RenderWindow* openSimpleWindow(winengine::MonitorInfo* monitor) {
    winengine::MonitorInfo mon_info;

    if (monitor == nullptr) {
        mon_info = winengine::WindowEngine::getInstance().getRandRInfo()[0];
    } else {
        mon_info = *monitor;
    }

// std::cout << mon_info.getPosition().x <<  ' ' << mon_info.getPosition().y
// << std::endl; std::cout << mon_info.getDimensions().x <<  ' ' <<
// mon_info.getDimensions().y << std::endl;
#undef None
    auto win = new sf::RenderWindow(
        sf::VideoMode(static_cast<int>(mon_info.getDimensions().x),
                      static_cast<int>(mon_info.getDimensions().y)),
        "X-papers", sf::Style::None);

    return win;
}

void test_video_handler(const char* path) {
    auto win_engine = winengine::WindowEngine::getInstance();
    auto main_mon = win_engine.getRandRInfo()[0];

    auto* win = openSimpleWindow(nullptr);

    winengine::VideoHandler my_movie(path, main_mon, win);
    my_movie.setFPSlimit(60);
    my_movie.enableVerticalSync();

    my_movie.start(handle_events);
}
