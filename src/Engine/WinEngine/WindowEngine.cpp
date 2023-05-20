////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "WindowEngine.hpp"
#include <spdlog/spdlog.h>
#include <exception>
#include <iostream>

namespace winengine {

WinEngineError::WinEngineError(WinEngineErrTypes type) noexcept
    : m_err_code(type) {}

const char* WinEngineError::what() const noexcept {
    switch (m_err_code) {
        case NoError: {
            return "No error. If this gets thrown, it's a bug.";
        }

        case NoX11UnixDir: {
            return "Can't open '/tmp/.X11-unix' directory";
        }

        case DisplayOpenError: {
            return "Can't open X Display";
        }

        case Unknown: {
            return "Unknown error";
        }

        case RandRVersionError: {
            return "RandR version unsupported. V >=1.5 is required";
        }

        case RandRUnaccessible: {
            return "Can't retrieve RandR version";
        }

        case TOTAL_ERRORS: {
            return "Fatal wrong exception";
        }
    }

    return "Unknown error";
}

MonitorInfo::MonitorInfo(const char* name,
                         pair_t position,
                         upair_t dimensions,
                         bool is_primary,
                         std::size_t monitor_index)
    : m_name(name),
      m_index(monitor_index),
      m_dimensions(dimensions),
      m_position(position),
      m_primary(is_primary) {
    spdlog::debug("monitor %100s: %i %i found\n", m_name, m_position.x,
                  m_position.y);
}

XrandrInfo::XrandrInfo(int active_only, const char* requested_display)
    : m_active_only(active_only), m_requested_display(requested_display) {
    auto errcode = scanMonitors();
    if (errcode != NoError) {
        THROW(WinEngineError(errcode));
    }
}

WinEngineErrTypes XrandrInfo::scanMonitors() {
    display_t* current_display = XOpenDisplay(m_requested_display);
    if (current_display == nullptr) {
        return DisplayOpenError;
    }

    // unnecessary external variables that will actually never be used lol
    int ext1 = 0, ext2 = 0;
    int major_version = 0, minor_version = 0;
    if (!XRRQueryExtension(current_display, &ext1, &ext2) ||
        !XRRQueryVersion(current_display, &major_version, &minor_version)) {
        XCloseDisplay(current_display);

        return RandRUnaccessible;
    }

    if (major_version <= 1 && minor_version < 5) {
        XCloseDisplay(current_display);

        return RandRVersionError;
    }

    const win_t root_win =
        RootWindow(current_display, DefaultScreen(current_display));
    Bool show_active_only = m_active_only;

    int mon_cnt = 0;
    XRRMonitorInfo* x_monitor_list =
        XRRGetMonitors(current_display, root_win, show_active_only, &mon_cnt);

    // casting to std::size_t to avoid saving it as 'int' (it doesn't make
    // sense since it can't be negative anyway)
    m_monitors_cnt = static_cast<std::size_t>(mon_cnt);
    m_monitors_info = std::vector<MonitorInfo>(m_monitors_cnt);

    for (std::size_t mon_idx = 0; mon_idx < m_monitors_cnt; ++mon_idx) {
        XRRMonitorInfo cur_monitor = x_monitor_list[mon_idx];
        MonitorInfo current_monitor_info(
            XGetAtomName(current_display, cur_monitor.name),
            {cur_monitor.x, cur_monitor.y},
            {static_cast<std::size_t>(cur_monitor.width),
             static_cast<std::size_t>(cur_monitor.height)},
            cur_monitor.primary, mon_idx);

        m_monitors_info[mon_idx] = current_monitor_info;
    }

    return NoError;
}

DisplayInfo::DisplayInfo(const std::string& display)
    : m_display_name(display.c_str()) {
    display_t* connection = XOpenDisplay(m_display_name);
    if (m_display_name == nullptr) {
        THROW(WinEngineError(DisplayOpenError));
    }

    int count = XScreenCount(connection);

    m_screen_count = static_cast<std::size_t>(count);
    spdlog::debug("Display %100s has %zu screens\n", m_display_name,
                  m_screen_count);

    m_dimensions = std::vector<upair_t>(m_screen_count);

    for (int screen_idx = 0; screen_idx < static_cast<int>(m_screen_count);
         ++screen_idx) {
        m_dimensions[screen_idx] = {
            static_cast<std::size_t>(XDisplayWidth(connection, screen_idx)),
            static_cast<std::size_t>(XDisplayHeight(connection, screen_idx))};
    }

    XCloseDisplay(connection);
}

const char* WindowEngine::X11_UNIX_DIRECTORY = "/tmp/.X11-unix";

WindowEngine::WindowEngine() {
    auto errcode = enumerateDisplays();
    if (errcode != NoError) {
        THROW(WinEngineError(errcode));
    }
}

WinEngineErrTypes WindowEngine::enumerateDisplays() {
    DIR* x11_unix_dir = opendir(X11_UNIX_DIRECTORY);
    if (x11_unix_dir == nullptr) {
        return NoX11UnixDir;
    }

    struct dirent* current_dirent;
    while ((current_dirent = readdir(x11_unix_dir)) != nullptr) {
        if (current_dirent->d_name[0] != 'X') {
            continue;
        }

        std::string display_name = ":";
        // appending the C string
        display_name += current_dirent->d_name + 1;

        DisplayInfo cur_display(display_name);
        // can't use indexation, need to use push_back since the amount of
        // displays can't be gotten beforehand

        m_displays.emplace_back(cur_display);
        ++m_displays_cnt;
    }

    closedir(x11_unix_dir);

    return NoError;
}

}  // namespace winengine
