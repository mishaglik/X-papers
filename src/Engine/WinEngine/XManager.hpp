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

class XWindowManager;

namespace detail {

class XWindowManagerDestroyer {
   private:
    XWindowManager* m_instance = nullptr;

   public:
    ~XWindowManagerDestroyer() {
        if (m_instance) {
            // TODO: ask ded about this warning
            // clang doesn't know if destructor will be public or not
            delete m_instance;
        }
    }

    void initialize(XWindowManager* ptr) { m_instance = ptr; }
};

}  // namespace detail

class XWindowManager {
   private:
    // c++ 17 only
    inline static XWindowManager* m_instance = nullptr;
    static detail::XWindowManagerDestroyer m_destroyer;

   public:
    XWindowManager(const char* display_name, int* screen_n) {
        m_connection = xcb_connect(display_name, screen_n);

        m_setup = xcb_get_setup(m_connection);
        m_iter = xcb_setup_roots_iterator(m_setup);

        m_screen = m_iter.data;
    }

    static XWindowManager& getInstance() {
        if (!m_instance) {
            m_instance = new XWindowManager();
            m_destroyer.initialize(m_instance);
        }

        return *m_instance;
    }

   public:
    // TODO: add asserts and exceptions
    XWindowHandler* addWindow(screen_coord_t x,
                              screen_coord_t y,
                              win_size_t width,
                              win_size_t height,
                              win_size_t border_width,
                              const void* value_list = nullptr) {
        window_t win_id = xcb_generate_id(m_connection);

        XWindowHandler* handler =
            new XWindowHandler(this, m_connection, win_id, m_screen, x, y,
                               width, height, 0, value_list);

        return handler;
    }

   protected:
    XWindowManager() {}
    XWindowManager(const XWindowManager&);
    XWindowManager& operator=(XWindowManager&);
    ~XWindowManager() {}

    friend class detail::XWindowManagerDestroyer;

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