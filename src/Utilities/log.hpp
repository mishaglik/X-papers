#ifndef UTILITIES_LOG_HPP
#define UTILITIES_LOG_HPP
#include <spdlog/spdlog.h>

/**
 * @brief Current component name. Passed by cmake.
 * 
 */
#ifndef XPPR_COMPONENT
#warn "XPPR_COMPONENT not set"
#define XPPR_COMPONENT "unknown"
#endif

/**
 * @brief List of all built components. 
 *  passed only to executables.
 */
#ifndef XPPR_COMPONENTS
#define XPPR_COMPONENTS {}
#endif


namespace xppr::log {

    static const char CUR_COMPONENT[] = XPPR_COMPONENT;

    static const char* COMPONENTS[] = {XPPR_COMPONENTS, nullptr}; /// Null-terminated list of all components. 

    namespace level {
        using namespace spdlog::level;
    }
    using spdlog::format_string_t;
    using spdlog::source_loc;

    template<typename... Args>
    static inline void log(level::level_enum lvl, format_string_t<Args...> fmt, Args &&... args)
    {
        spdlog::get(CUR_COMPONENT)->log(source_loc{}, lvl, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void debug(format_string_t<Args...> fmt, Args &&... args)
    {
        spdlog::get(CUR_COMPONENT)->debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void info(format_string_t<Args...> fmt, Args &&... args)
    {
        spdlog::get(CUR_COMPONENT)->info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void warn(format_string_t<Args...> fmt, Args &&... args)
    {
        spdlog::get(CUR_COMPONENT)->warn(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void error(format_string_t<Args...> fmt, Args &&... args)
    {
        spdlog::get(CUR_COMPONENT)->error(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void critical(format_string_t<Args...> fmt, Args &&... args)
    {
        spdlog::get(CUR_COMPONENT)->critical(fmt, std::forward<Args>(args)...);
    }

    void init_logger(const char* components[] = COMPONENTS) noexcept;    
}

#endif /* UTILITIES_LOG_HPP */
