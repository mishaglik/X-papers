#include "XManager.hpp"

namespace winengine {

XWindowManager::XWindowManager(const char* display_name, int* screen_n) {
    m_connection = xcb_connect(display_name, screen_n);

    m_setup = xcb_get_setup(m_connection);
    m_iter = xcb_setup_roots_iterator(m_setup);

    m_screen = m_iter.data;
}

std::shared_ptr<XWindowManager> XWindowManager::getInstance() {
    // nullptr as default, values should be hardcoded, we don't need anything
    // else other than nullptr there
    [[clang::no_destroy]] static std::shared_ptr<XWindowManager> instance{
        new XWindowManager(nullptr, nullptr)};

    return instance;
}

XWindowHandler* XWindowManager::addWindow(screen_coord_t x,
                                          screen_coord_t y,
                                          win_size_t     width,
                                          win_size_t     height,
                                          win_size_t     border_width,
                                          uint32_t       value_mask,
                                          const void*    value_list) {
    window_t win_id = xcb_generate_id(m_connection);

    XWindowHandler* handler =
        new XWindowHandler(this, win_id, m_screen, x, y, width,
                           height, border_width, value_mask, value_list);

    ++m_total_windows;
    return handler;
}

}  // namespace winengine
