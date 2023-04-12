#include "log.hpp"
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace xppr::log {

    class module_format_flag : public spdlog::custom_flag_formatter
    {
    public:
        void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override
        {
            std::string some_txt = "custom-flag";
            dest.append(some_txt.data(), some_txt.data() + some_txt.size());
        }

        std::unique_ptr<custom_flag_formatter> clone() const override
        {
            return spdlog::details::make_unique<module_format_flag>();
        }
    };




    void init_logger(const char* components[]) noexcept
    {
        try {
            auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
            console_sink->set_level(spdlog::level::trace);
            console_sink->set_pattern("[%x %T] [%n] <%^%l%$> %v");

            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/x-papers.log", 1024 * 1024, 5, true);
            file_sink->set_level(spdlog::level::trace);
            file_sink->set_pattern("[%x %T] [%n] <%l>  %v");

            spdlog::sinks_init_list sink_list = { file_sink, console_sink };
            while(*components) {
                spdlog::register_logger(std::make_shared<spdlog::logger>(*(components), spdlog::sinks_init_list({console_sink, file_sink})));
                spdlog::get(*components)->info("{} logger initted", *components);
                components++;
            }
            spdlog::set_default_logger(std::make_shared<spdlog::logger>("default", spdlog::sinks_init_list({console_sink, file_sink})));
            info("All loggers initted successfully");
        } catch(const spdlog::spdlog_ex& ex) {
            fprintf(stderr, "Log initialization failed: %s", ex.what());
            std::exit(-1);
        }
    }

}