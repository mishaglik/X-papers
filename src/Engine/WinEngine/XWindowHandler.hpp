#ifndef X11_WINDOW_HANDLER_HPP
#define X11_WINDOW_HANDLER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>
#include <xcb/xproto.h>
#include <Utilities/optional.hpp>
#include <cstring>
#include <memory>

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

class XWindowHandler {
    friend class XWindowManager;

   private:
    XWindowHandler(XWindowManager* manager,
                             connection_t* connection,
                             window_t window_id,
                             screen_t* screen,
                             screen_coord_t x,
                             screen_coord_t y,
                             win_size_t width,
                             win_size_t height,
                             uint32_t value_mask = 0,
                             const void* value_list = nullptr)
        : m_connection(connection), m_manager(manager), m_win_id(window_id) {
        assert("INVALID connection pointer" && m_connection);
        assert("INVALID window id" && m_win_id != -1);
        assert("INVALID screen pointer" && screen);

        xcb_create_window(connection, XCB_COPY_FROM_PARENT, m_win_id,
                          screen->root, x, y, width, height, 0,
                          XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
                          value_mask, value_list);
    }

    ~XWindowHandler() = default;

   public:
    cookie_t showWindow() {
        assert("INVALID connection pointer" && m_connection);
        assert("INVALID window id" && m_win_id != -1);

        return xcb_map_window(m_connection, m_win_id);
    }

    cookie_t changeProperty(xcb_atom_t property,
                            xcb_atom_t type,
                            const void* data) {
        assert("INVALID data pointer" && data);

        return xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE,
                                   m_win_id, property, type, 8,
                                   strlen((char*)data), data);
    }

   private:
    window_t m_win_id = 0;
    connection_t* m_connection = nullptr;
    XWindowManager* m_manager = nullptr;
};

}  // namespace winengine

#endif  // !X11_WINDOW_HANDLER_HPP