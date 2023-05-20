#include "XDisplayHandler.hpp"
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

    ++m_total_windows;

    return new_handler;
}

XWindowHandler* XDisplayHandler::addWindow(win_t id) {
    XWindowHandler* new_handler =
        new XWindowHandler(this, id);
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
    [[clang::no_destroy]] static std::shared_ptr<XDisplayHandler> instance{
        new XDisplayHandler(nullptr)};

    return instance;
}

}  // namespace winengine
