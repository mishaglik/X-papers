#include "engine.hpp"
#include <iostream>
#include "WinEngine/proto.hpp"
#include "WindowEngine.hpp"
#include "XDisplayHandler.hpp"

void test_open_window() {
    auto engine = winengine::WindowEngine::getInstance();
    for (std::size_t i = 0; i < engine.getTotalDisplays(); ++i) {
        std::cout << i << ' ' << engine[i].getScreenCount() << std::endl;
    }

    std::cout << engine.getUnixDirectory() << std::endl;

    auto randrinfo = engine.getRandRInfo();

    for (std::size_t i = 0; i < randrinfo.getMonitorsCount(); ++i) {
        std::cout << i << ' ' << randrinfo[i].getName() << ' ' << randrinfo[i].getIndex() << std::endl;
    }
    /*
    winengine::win_attr_t attrib;
    attrib.override_redirect = True;

    auto manager = winengine::XDisplayHandler::getInstance();
    auto win = manager->addWindow({0, 0}, {1920, 1080}, 0, winengine::Parent,
                                  CopyFromParent, 0, &attrib);

    auto wintype = manager->createAtom("_NET_WM_WINDOW_TYPE", False);
    auto desktop = manager->createAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);

    win->changeProperty(wintype, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)&desktop, 1);

    // auto wm_state = manager->createAtom("_NET_WM_STATE", true);
    // auto wm_fullscreen = manager->createAtom("_NET_WM_STATE_FULLSCREEN",
    // true);

    // win->changeProperty(wm_state, XA_ATOM, 32, PropModeReplace,
    //                     (unsigned char*)&wm_fullscreen, 1);

    win->setBackground(0xFFFFFF);  // { width, heigth }

    win->clear();
    win->show();

    while (1) {
        XEvent event;

        manager->nextEvent(&event);
    }

    win->hide();
    manager->closeDisplay();
    */
    return;
}
