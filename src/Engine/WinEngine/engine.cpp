#include "engine.hpp"
#include "WinEngine/proto.hpp"
#include "XManager.hpp"

void test_open_window() {
    winengine::win_attr_t attrib;
    attrib.override_redirect = True;

    auto manager = winengine::XWindowManager::getInstance();
    auto win = manager->addWindow({0, 0}, {1920, 1080}, 0, winengine::Parent,
                                  CopyFromParent, 0, &attrib);

    auto wintype = manager->createAtom("_NET_WM_WINDOW_TYPE", False);
    auto desktop = manager->createAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);

    win->changeProperty(wintype, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)&desktop, 1);

    auto wm_state = manager->createAtom("_NET_WM_STATE", true);
    auto wm_fullscreen = manager->createAtom("_NET_WM_STATE_FULLSCREEN", true);

    win->changeProperty(wm_state, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)&wm_fullscreen, 1);

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
