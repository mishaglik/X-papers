#include "XDisplayHandler.hpp"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include "WinEngine/XWindowHandler.hpp"

#include "WinEngine/proto.hpp"

namespace winengine {

XDisplayHandler::XDisplayHandler(const char* display_name) {
    m_display = XOpenDisplay(display_name);
    m_screen = DefaultScreen(m_display);
}

XWindowHandler* XDisplayHandler::addWindow(pair_t coords,
                                           pair_t size,
                                           int border_width,
                                           class_t class_type,
                                           visual_t* visual,
                                           uint32_t mask,
                                           win_attr_t* attributes) {
    XWindowHandler* new_handler =
        new XWindowHandler(this, m_display, coords, size, border_width,
                           class_type, visual, mask, attributes);
    m_windows_list.push_back(new_handler);

    ++m_total_windows;

    return new_handler;
}

XWindowHandler* XDisplayHandler::addWindow(win_t id) {
    XWindowHandler* new_handler = new XWindowHandler(this, id);
    m_windows_list.push_back(new_handler);

    ++m_total_windows;

    return new_handler;
}

atom_t XDisplayHandler::createAtom(const char* atom_name, int only_if_exists) {
    return XInternAtom(m_display, atom_name, only_if_exists);
}

int XDisplayHandler::flush() {
    return XFlush(m_display);
}

int XDisplayHandler::changeWindowProperty(XWindowHandler& handler,
                                          atom_t property,
                                          atom_t type,
                                          int format,
                                          int mode,
                                          const uint8_t* data,
                                          int nelements) {
    return handler.changeProperty(property, type, format, mode, data,
                                  nelements);
}

std::shared_ptr<XDisplayHandler> XDisplayHandler::getInstance() {
    // nullptr as default, values should be hardcoded, we don't need anything
    // else other than nullptr there
    static std::shared_ptr<XDisplayHandler> instance{
        new XDisplayHandler(nullptr)};

    return instance;
}

bool XDisplayHandler::setBackgroundProperties(win_t target_id) {
    auto manager = winengine::XDisplayHandler::getInstance();
    return setBackgroundProperties(manager->addWindow(target_id));
}

bool XDisplayHandler::setBackgroundProperties(XWindowHandler* target_win) {
    if (target_win == nullptr) {
        return false;
    }

    auto manager = winengine::XDisplayHandler::getInstance();

    auto wintype = manager->createAtom("_NET_WM_WINDOW_TYPE", False);
    auto desktop = manager->createAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);

    target_win->changeProperty(wintype, XA_ATOM, 32, PropModeReplace,
                               reinterpret_cast<unsigned char*>(&desktop), 1);

    unsigned long prop = 0xFFFFFFFF;  // note long! even if long is 64 bit

    target_win->changeProperty(manager->createAtom("_NET_WM_DESKTOP", 1),
                               XA_CARDINAL,  // note CARDINAL not ATOM
                               32, PropModeReplace, reinterpret_cast<unsigned char*>(&prop),
                               1);  // note 1
    return true;
}

}  // namespace winengine
