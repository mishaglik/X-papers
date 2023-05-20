#include <SFML/Window/Event.hpp>
#include <sfeMovie/Movie.hpp>

#include <X11/X.h>
#include <X11/Xatom.h>
#include <algorithm>
#include <iostream>
#include "WinEngine/VideoHandler.hpp"
#include "WinEngine/WindowEngine.hpp"
#include "WinEngine/XDisplayHandler.hpp"
#include "WinEngine/proto.hpp"
#include "glfwRender.hpp"
#include "engine.hpp"

#include <X11/Xlib.h>

void test_gl(const char* filename) {
    winengine::GlfwController controller;
    
    controller.Start(filename);
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
        handler->setView(sf::View(sf::FloatRect(
            0, 0, (float)handler->getSize().x, (float)handler->getSize().y)));
    }
}

void test_video_handler(const char* path) {
    auto win_engine = winengine::WindowEngine::getInstance();
    auto main_mon = win_engine.getRandRInfo()[0];
    // std::cout << main_mon.getX() << ' ' << main_mon.getY() << std::endl;
    // std::cout << main_mon.getWidth() << ' ' << main_mon.getHeight() <<
    // std::endl;

    winengine::VideoHandler my_movie(path, main_mon);
    my_movie.setFPSlimit(60);
    my_movie.enableVerticalSync();

    my_movie.start(handle_events);
}

void test_render_wallpaper(const char* path = nullptr) {
    if (path == nullptr) {
        std::cout << "Error: no path provided" << std::endl;
        return;
    }

    std::string path_string = std::string(path);

    sfe::Movie video_render_obj;
    if (!video_render_obj.openFromFile(path_string)) {
        return;
    }

    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    float width = std::min(static_cast<float>(desktopMode.width),
                           video_render_obj.getSize().x);
    float height = std::min(static_cast<float>(desktopMode.height),
                            video_render_obj.getSize().y);
    std::cout << desktopMode.width << ' ' << desktopMode.height << std::endl;

    if (width * height < 1.f) {
        width = std::max(width, 250.f);
        height = std::max(height, 40.f);
    }

#undef None
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "X-papers",
                            sf::Style::None);
    window.setPosition({0, 0});
    //
    winengine::win_attr_t attrib;
    attrib.override_redirect = True;

    auto manager = winengine::XDisplayHandler::getInstance();
    auto win = manager->addWindow(window.getSystemHandle());

    auto wintype = manager->createAtom("_NET_WM_WINDOW_TYPE", False);
    auto desktop = manager->createAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);

    win->changeProperty(wintype, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)&desktop, 1);

    unsigned int ints[2];
    ints[0] = 0xFFFFFFFF;
    ints[1] = 2;

    auto wm_desktop = manager->createAtom("_NET_WM_DESKTOP", False);

    win->changeProperty(wm_desktop, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)ints, 2);

    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    std::cout << width << ' ' << height;
    video_render_obj.fit(0, 0, 1920, 1080);
    video_render_obj.play();

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed ||
                (ev.type == sf::Event::KeyPressed &&
                 ev.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if (ev.type == sf::Event::Resized) {
                video_render_obj.fit(0, 0, window.getSize().x,
                                     window.getSize().y);
                window.setView(
                    sf::View(sf::FloatRect(0, 0, (float)window.getSize().x,
                                           (float)window.getSize().y)));
            }
        }

        video_render_obj.update();

        window.clear();
        window.draw(video_render_obj);
        window.display();
    }

    manager->closeDisplay();
}

winengine::XWindowHandler* test_open_window() {
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
                        (unsigned char*)&desktop, 1);

    unsigned int ints[2];
    ints[0] = 0xFFFFFFFF;
    ints[1] = 2;


    auto wm_desktop = manager->createAtom("_NET_WM_DESKTOP", True);

    win->changeProperty(wm_desktop, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)ints, 2);

    // auto wm_state = manager->createAtom("_NET_WM_STATE", true);
    // auto wm_fullscreen = manager->createAtom("_NET_WM_STATE_FULLSCREEN",
    // true);

    // win->changeProperty(wm_state, XA_ATOM, 32, PropModeReplace,
    //                     (unsigned char*)&wm_fullscreen, 1);

    win->changeProperty(wm_desktop, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)ints, 2);

    // auto wm_state = manager->createAtom("_NET_WM_STATE", true);
    // auto wm_fullscreen = manager->createAtom("_NET_WM_STATE_FULLSCREEN",
    // true);

    // win->changeProperty(wm_state, XA_ATOM, 32, PropModeReplace,
    //                     (unsigned char*)&wm_fullscreen, 1);

    win->setBackground(0xFFFFFF);

    win->clear();
    win->show();

    return win;
}
