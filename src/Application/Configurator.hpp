#ifndef APPLICATION_CONFIGURATOR_HPP
#define APPLICATION_CONFIGURATOR_HPP

#include <cstddef>

#include <WallpaperEngine/WallpaperEngine.hpp>

class Configurator {
   public:
    Configurator() = default;

    void genConfig(int argc, const char* argv[]);

    static Configurator fromArguments(int argc, const char* argv[]) {
        Configurator cfg{};
        cfg.genConfig(argc, argv);
        return cfg;
    }

    bool isDemon() { return m_daemonize; }

   private:
    using ParseCallback = size_t (Configurator::*)(const char**, size_t);

    struct ParseEntry {
        const char* m_key;
        const char* m_descr;
        ParseCallback m_callback;
    };

    size_t parseArgument(const char* args[], size_t size);

    template <bool Configurator::*attr, bool value = true>
    size_t genericSet(const char**, size_t) {
        this->*attr = value;
        return 1;
    }

    size_t help(const char**, size_t);

    bool isDaemonizing() const { return m_daemonize; }

   private:
    const char* m_program_name = "xpapers";
    const char* m_program_path = nullptr;
    bool m_daemonize = false;

    static const ParseEntry CommandArguments[];
};

/**
 * @brief Make programm run as daemon
 * 
 */
void daemonize();

/**
 * @brief Setup signal handlers.
 * 
 * @param app - current application.
 */
void setupSignals(xppr::wpeng::WallpaperEngine* app);

#endif /* APPLICATION_CONFIGURATOR_HPP */
