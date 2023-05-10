#include <sfeMovie/Movie.hpp>

#include "engine.hpp"
#include <X11/X.h>
#include <X11/Xatom.h>
#include <algorithm>
#include <iostream>
#include "WinEngine/XDisplayHandler.hpp"
#include "WinEngine/proto.hpp"

#include <X11/extensions/Xrandr.h>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/WindowStyle.hpp>

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
    float width =
        std::min(static_cast<float>(desktopMode.width), video_render_obj.getSize().x);
    float height =
        std::min(static_cast<float>(desktopMode.height), video_render_obj.getSize().y);
    std::cout << desktopMode.width << ' ' << desktopMode.height << std::endl;

    if (width * height < 1.f) {
        width = std::max(width, 250.f);
        height = std::max(height, 40.f);
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "X-papers", 0);
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

    auto wm_desktop = manager->createAtom("_NET_WM_DESKTOP", True);

    win->changeProperty(wm_desktop, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)ints, 2);

    //
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    video_render_obj.fit(0, 0, width, height);
    video_render_obj.play();

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed ||
                (ev.type == sf::Event::KeyPressed &&
                 ev.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if (ev.type == sf::Event::KeyPressed) {
                switch (ev.key.code) {
                    case sf::Keyboard::Space:
                        if (video_render_obj.getStatus() == sfe::Playing)
                            video_render_obj.pause();
                        else
                            video_render_obj.play();
                        break;
                    default:
                        break;
                }
            } else if (ev.type == sf::Event::Resized) {
                video_render_obj.fit(0, 0, window.getSize().x, window.getSize().y);
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

    win->setBackground(0xFFFFFF);

    win->clear();
    win->show();

    while (1) {
        XEvent event;

        manager->nextEvent(&event);
    }

    win->hide();
    manager->closeDisplay();

    return;
}
