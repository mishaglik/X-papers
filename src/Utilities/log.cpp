#include "log.hpp"
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace xppr::log {

    static std::shared_ptr<spdlog::sinks::stderr_color_sink_mt > ConsoleSink;
    static std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> FileSink;

    void init_logger(const char* components[]) noexcept
    {
        if(ConsoleSink != nullptr) {
            return;
        }

        try {
            ConsoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
            ConsoleSink->set_level(spdlog::level::critical);
            ConsoleSink->set_pattern("[%x %T] [%n] <%^%l%$> %v");

            FileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/x-papers.log", 1024 * 1024, 5, true);
            FileSink->set_level(spdlog::level::trace);
            FileSink->set_pattern("[%x %T.%e] [%n] <%l>  %v");

            while(*components) {
                spdlog::register_logger(std::make_shared<spdlog::logger>(*(components), spdlog::sinks_init_list({ConsoleSink, FileSink})));
                spdlog::get(*components)->info("{} logger initted", *components);
                components++;
            }
            spdlog::set_default_logger(std::make_shared<spdlog::logger>("default", spdlog::sinks_init_list({ConsoleSink, FileSink})));
            info("All loggers initted successfully");
        } catch(const spdlog::spdlog_ex& ex) {
            fprintf(stderr, "Log initialization failed: %s", ex.what());
            std::exit(-1);
        }
    }

    void add_logger(const char* component) noexcept {
        try {
            spdlog::register_logger(std::make_shared<spdlog::logger>(component, spdlog::sinks_init_list({ConsoleSink, FileSink})));
            spdlog::get(component)->info("{} logger initted", component);            
        } catch(const spdlog::spdlog_ex& ex) {
            fprintf(stderr, "Log initialization failed: %s", ex.what());
            std::exit(-1);
        }
    }

    std::shared_ptr<spdlog::logger> get_logger(const char* component) noexcept {
        return spdlog::get(component);
    }

}
