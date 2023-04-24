#include "XManager.hpp"
#include <X11/Xlib.h>
#include "WinEngine/XWindowHandler.hpp"

namespace winengine {

XWindowManager::XWindowManager(const char* display_name) {
    m_display = XOpenDisplay(display_name);
    m_screen = DefaultScreen(m_display);
}

XWindowHandler* XWindowManager::addWindow(pair_t coords,
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

atom_t XWindowManager::createAtom(const char* atom_name, int only_if_exists) {
    return XInternAtom(m_display, atom_name, only_if_exists);
}

int XWindowManager::flush() {
    return XFlush(m_display);
}

int XWindowManager::changeWindowProperty(XWindowHandler& handler,
                                         atom_t property,
                                         atom_t type,
                                         int format,
                                         int mode,
                                         const uint8_t* data,
                                         int nelements) {
    return handler.changeProperty(property, type, format, mode, data,
                                  nelements);
}

std::shared_ptr<XWindowManager> XWindowManager::getInstance() {
    // nullptr as default, values should be hardcoded, we don't need anything
    // else other than nullptr there
    [[clang::no_destroy]] static std::shared_ptr<XWindowManager> instance{
        new XWindowManager(nullptr)};

    return instance;
}

}  // namespace winengine
