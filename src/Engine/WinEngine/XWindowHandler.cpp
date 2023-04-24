#include "XWindowHandler.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdio>
#include "WinEngine/proto.hpp"
#include "XManager.hpp"

namespace winengine {

XWindowHandler::XWindowHandler(XWindowManager* manager,
                               display_t* display,
                               pair_t coords,
                               pair_t size,
                               int border_width,
                               class_t class_type,
                               visual_t* visual,
                               uint32_t mask,
                               win_attr_t* attributes)
    : m_manager(manager) {
    assert("INVALID XWindowManager pointer" && manager);
    assert("INVALID display pointer" && display);

    m_win_id = XCreateWindow(display, XDefaultRootWindow(display), coords.x,
                             coords.y, size.x, size.y, border_width,
                             XDefaultDepth(display, manager->m_screen),
                             class_type, visual, mask, attributes);
}

int XWindowHandler::show() {
    return XMapWindow(m_manager->m_display, m_win_id);
}

int XWindowHandler::hide() {
    return XUnmapWindow(m_manager->m_display, m_win_id);
}

int XWindowHandler::setBackground(const uint64_t color) {
    return XSetWindowBackground(m_manager->m_display, m_win_id, color);
}

void XWindowHandler::removeTakeFocusProtocol() {
    atom_t wm_take_focus_atom;
    atom_t* protocols = nullptr;
    int total_protocols = 0;

    wm_take_focus_atom =
        XInternAtom(m_manager->m_display, "WM_TAKE_FOCUS", False);
    if (XGetWMProtocols(m_manager->m_display, m_win_id, &protocols,
                        &total_protocols) != 0) {
        atom_t* new_list = new atom_t[total_protocols + 1];
        int new_count = 0;

        for (int protocole_number = 0; protocole_number < total_protocols;
             protocole_number++) {
            if (protocols[protocole_number] == wm_take_focus_atom) {
                continue;
            }

            new_list[new_count++] = protocols[protocole_number];
        }

        XSetWMProtocols(m_manager->m_display, m_win_id, new_list, new_count);
        XFree(protocols);

        delete[] new_list;
    }
}

int XWindowHandler::clear() {
    return XClearWindow(m_manager->m_display, m_win_id);
}

int XWindowHandler::setHints(wm_hints_t* hints) {
    assert("INVALID hints pointer" && hints != nullptr);
    return XSetWMHints(m_manager->m_display, m_win_id, hints);
}

int XWindowHandler::changeProperty(atom_t property,
                                   atom_t type,
                                   int format,
                                   int mode,
                                   const uint8_t* data,
                                   int nelements) {
    assert("INVALID data pointer" && data);
    assert("INVALID manager" && m_manager);
    assert("INVALID manager display" && m_manager->m_display);
    return XChangeProperty(m_manager->m_display, m_win_id, property, type,
                           format, mode, data, nelements);
}

}  // namespace winengine
