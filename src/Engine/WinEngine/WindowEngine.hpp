#ifndef X11_WINDOW_ENGINE_HPP
#define X11_WINDOW_ENGINE_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <X11/Xlib.h>
#include <dirent.h>
#include <spdlog/spdlog.h>
#include <exception>
#include <stdexcept>
#include <system_error>
#include "Utilities/pair.hpp"
#include "proto.hpp"

namespace winengine {

enum WinEngineErrTypes {
    NoError,
    NoX11UnixDir,
    DisplayOpenError,
    RandRVersionError,
    RandRUnaccessible,
    Unknown,
    TOTAL_ERRORS  // KEEP THIS ERR_COUNTER IN THE END
};

class WinEngineError : public std::exception {
   public:
    WinEngineError(WinEngineErrTypes type) noexcept;

    const char* what() const noexcept override;

    WinEngineErrTypes getErrorType() const { return m_err_code; }

   private:
    WinEngineErrTypes m_err_code = Unknown;
};

class MonitorInfo {
    friend class XrandrInfo;

   public:
    // Is not needed, but required to be able to add it to std::vector
    explicit MonitorInfo() = default;

   private:
    MonitorInfo(const char* name,
                pair_t position,
                upair_t dimensions,
                bool is_primary,
                std::size_t monitor_index);

   public:
    const char* getName() const {
        assert("INVALID monitor name" && m_name != nullptr);
        return m_name;
    }

    upair_t getDimensions() const { return m_dimensions; }

    pair_t getPosition() const { return m_position; }

    int getX() const { return m_position.x; }

    int getY() const { return m_position.y; }

    std::size_t getWidth() const { return m_dimensions.x; }

    std::size_t getHeight() const { return m_dimensions.y; }

    std::size_t getIndex() const { return m_index; }

    std::size_t isPrimary() const { return m_primary; }

   private:
    const char* m_name = nullptr;
    std::size_t m_index = 0;  // zero here actually means the right thing, 0
                              // monitor index means MAIN monitor

    upair_t m_dimensions;
    pair_t m_position;

    bool m_primary = false;
};

class XrandrInfo {
    friend class WindowEngine;

   public:
    const MonitorInfo& operator[](const std::size_t index) const {
        return m_monitors_info[index];
    }

    std::size_t getMonitorsCount() const {
        return m_monitors_cnt;
    }

   private:
    explicit XrandrInfo(int active_only = True,
                        const char* requested_display = nullptr);

    WinEngineErrTypes scanMonitors();

    // it would make sense to make this std::size_t but XRRGetMonitors require
    // 'int'
    std::size_t m_monitors_cnt = 0;

    // staying with xlib typedefs here
    Bool m_active_only = True;
    const char* m_requested_display = nullptr;

    std::vector<MonitorInfo> m_monitors_info;
};

class DisplayInfo {
    friend class WindowEngine;

   private:
    explicit DisplayInfo() = default;

    DisplayInfo(const std::string& display);

   public:
    std::size_t getScreenCount() const { return m_screen_count; }

    const std::vector<upair_t>& getDimensions() const { return m_dimensions; }

    const char* getDisplayName() const {
        // CRINGE, added this function just in case
        return m_display_name;
    }

   private:
    const char* m_display_name = nullptr;

    std::size_t m_screen_count = 0;
    std::vector<upair_t> m_dimensions;
};

class WindowEngine {
   private:
    explicit WindowEngine();

   public:
    static WindowEngine& getInstance() {
        [[clang::no_destroy]] static WindowEngine current_engine;
        return current_engine;
    }

    const DisplayInfo& operator[](const std::size_t index) const { return m_displays[index]; }

    XrandrInfo& getRandRInfo() { return m_info; }

    // I have no idea why would anyone ever use that
    std::string getUnixDirectory() const {
        return std::string(X11_UNIX_DIRECTORY);
    }

    std::size_t getTotalDisplays() const {
        return m_displays_cnt;
    }

   private:
    WinEngineErrTypes enumerateDisplays();

   private:
    static const char* X11_UNIX_DIRECTORY;

    std::size_t m_displays_cnt = 0;
    std::vector<DisplayInfo> m_displays;
    XrandrInfo m_info;
};

}  // namespace winengine

#endif  // !X11_WINDOW_ENGINE_HPP
