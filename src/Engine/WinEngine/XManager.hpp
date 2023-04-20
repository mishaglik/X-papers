#ifndef X11_XMANAGER_HPP
#define X11_XMANAGER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
/* DON'T CHANGE THE ORDER, IT WILL BREAK */
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include "XWindowHandler.hpp"

namespace winengine {

using connection_t = xcb_connection_t;
using setup_t = xcb_setup_t;
using screen_t = xcb_screen_t;
using screen_iter_t = xcb_screen_iterator_t;
using window_t = xcb_window_t;
using visual_t = xcb_visualid_t;
using cookie_t = xcb_void_cookie_t;

using screen_coord_t = int16_t;
using win_size_t = uint16_t;


class XWindowHandler;

class XWindowManager {
    private:
    XWindowManager(const char* display_name, int* screen_n);

    public:
    static std::shared_ptr<XWindowManager> getInstance();
    
    XWindowHandler* addWindow(screen_coord_t x,
                              screen_coord_t y,
                              win_size_t width,
                              win_size_t height,
                              win_size_t border_width = 0,
                              uint32_t value_mask = 0,
                              const void* value_list = nullptr);

    std::size_t getTotalWindows() const {
        return m_total_windows;
    }

    XWindowManager(XWindowManager const&) = delete;
    XWindowManager& operator=(XWindowManager const&) = delete;

    friend class XWindowHandler;

   private:
    std::size_t m_total_windows = 0;

    screen_t* m_screen = nullptr;
    screen_iter_t m_iter;
    const setup_t* m_setup = nullptr;
    connection_t* m_connection = nullptr;

    // TODO: add windows to map
    // std::map<window_t, XWindowHandler*> m_win_map;
};

}  // namespace winengine

#endif  // !X11_XMANAGER_HPP
