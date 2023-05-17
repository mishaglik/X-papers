#ifndef UTILITIES_LOG_HPP
#define UTILITIES_LOG_HPP
#include <spdlog/spdlog.h>

/**
 * @brief Current component name. Passed by cmake.
 *
 */
#ifndef XPPR_COMPONENT
#warning "XPPR_COMPONENT not set"
#define XPPR_COMPONENT "unknown"
#endif

/**
 * @brief List of all built components.
 *  passed only to executables.
 */
#ifndef XPPR_COMPONENTS
#define XPPR_COMPONENTS \
    {}
#endif

namespace xppr::log {

    static const char CUR_COMPONENT[] = XPPR_COMPONENT;

static const char* COMPONENTS[] = {
    XPPR_COMPONENTS, nullptr};  /// Null-terminated list of all components.

namespace level {
using namespace spdlog::level;
}
using spdlog::format_string_t;
using spdlog::source_loc;
std::shared_ptr<spdlog::logger> get_logger(const char* component = CUR_COMPONENT) noexcept;

template <typename... Args>
static inline void log(level::level_enum lvl,
                       format_string_t<Args...> fmt,
                       Args&&... args) {
    get_logger()
        ->log(source_loc{}, lvl, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
static inline void debug(format_string_t<Args...> fmt, Args&&... args) {
    get_logger()->debug(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
static inline void info(format_string_t<Args...> fmt, Args&&... args) {
    get_logger()->info(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
static inline void warn(format_string_t<Args...> fmt, Args&&... args) {
    get_logger()->warn(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
static inline void error(format_string_t<Args...> fmt, Args&&... args) {
    get_logger()->error(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
static inline void critical(format_string_t<Args...> fmt, Args&&... args) {
    get_logger()->critical(fmt, std::forward<Args>(args)...);
}

void init_logger(const char* components[] = COMPONENTS) noexcept;
void add_logger(const char* component = CUR_COMPONENT) noexcept;
}  // namespace xppr::log

#endif /* UTILITIES_LOG_HPP */
