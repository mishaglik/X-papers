#ifndef X11_XMANAGER_HPP
#define X11_XMANAGER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
/* DON'T CHANGE THE ORDER, IT WILL BREAK */
#include <X11/Xlib.h>
#include "WinEngine/WindowEngine.hpp"
#include "WinEngine/proto.hpp"
#include "XWindowHandler.hpp"

namespace winengine {

class XWindowHandler;

class XDisplayHandler {
    friend class WindowEngine;

   private:
    XDisplayHandler(const char* display_name = nullptr);

   public:
    static std::shared_ptr<XDisplayHandler> getInstance();

    XWindowHandler* addWindow(pair_t coords,
                              pair_t size,
                              int border_width = 0,
                              class_t class_type = class_t::Parent,
                              visual_t* visual = nullptr,
                              uint32_t mask = 0,
                              win_attr_t* attributes = nullptr);

    atom_t createAtom(const char* atom_name, int only_if_exists);

    int flush();

    int nextEvent(event_t* event) { return XNextEvent(m_display, event); }

    int closeDisplay() { return XCloseDisplay(m_display); }

    int changeWindowProperty(XWindowHandler& handler,
                             atom_t property,
                             atom_t type,
                             int format,
                             int mode,
                             const uint8_t* data,
                             int nelements);

    std::size_t getTotalWindows() const { return m_total_windows; }

    XDisplayHandler(XDisplayHandler const&) = delete;
    XDisplayHandler& operator=(XDisplayHandler const&) = delete;

    friend class XWindowHandler;

   private:
    std::size_t m_total_windows = 0;

    display_t* m_display = nullptr;
    screen_t m_screen = 0;
};

}  // namespace winengine

#endif  // !X11_XMANAGER_HPP
