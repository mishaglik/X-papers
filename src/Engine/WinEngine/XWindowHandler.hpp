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
                             window_t window_id,
                             screen_t* screen,
                             screen_coord_t x,
                             screen_coord_t y,
                             win_size_t width,
                             win_size_t height,
                             win_size_t border_width = 0,
                             uint32_t value_mask = 0,
                             const void* value_list = nullptr);

    ~XWindowHandler() = default;

   public:
    cookie_t showWindow();

    cookie_t changeProperty(xcb_atom_t property,
                            xcb_atom_t type,
                            const void* data);

   private:
    window_t m_win_id = 0;
    XWindowManager* m_manager = nullptr;
};

}  // namespace winengine

#endif  // !X11_WINDOW_HANDLER_HPP