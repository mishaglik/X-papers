#include "XWindowHandler.hpp"
#include "XManager.hpp"

namespace winengine {

XWindowHandler::XWindowHandler(XWindowManager* manager,
                               window_t window_id,
                               screen_t* screen,
                               screen_coord_t x,
                               screen_coord_t y,
                               win_size_t width,
                               win_size_t height,
                               win_size_t border_width,
                               uint32_t value_mask,
                               const void* value_list)
    : m_win_id(window_id), m_manager(manager) {
    assert("INVALID connection pointer" && m_manager->m_connection);
    assert("INVALID window id" && static_cast<int>(m_win_id) != -1);
    assert("INVALID screen pointer" && screen);

    xcb_create_window(m_manager->m_connection, XCB_COPY_FROM_PARENT, m_win_id,
                      screen->root, x, y, width, height, border_width,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
                      value_mask, value_list);
}

cookie_t XWindowHandler::showWindow() {
    assert("INVALID connection pointer" && m_manager->m_connection);
    assert("INVALID window id" && static_cast<int>(m_win_id) != -1);

    return xcb_map_window(m_manager->m_connection, m_win_id);
}

cookie_t XWindowHandler::changeProperty(xcb_atom_t property,
                                        xcb_atom_t type,
                                        const void* data) {
    assert("INVALID data pointer" && data);

    return xcb_change_property(
        m_manager->m_connection, XCB_PROP_MODE_REPLACE, m_win_id, property,
        type, 8, static_cast<uint32_t>(strlen(static_cast<const char*>(data))),
        data);
}

}  // namespace winengine