#ifndef APPLICATION_CONFIGURATOR_HPP
#define APPLICATION_CONFIGURATOR_HPP

#include <cstddef>

#include <WallpaperEngine/WallpaperEngine.hpp>
#include <cstdint>
#include <cstdio>

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

    template <uint32_t Configurator::*attr>
    size_t genericSet(const char**argv, size_t) {
        uint32_t x = 0;
        sscanf(argv[1], "%u", &x);
        this->*attr = x;
        return 2;
    }

    size_t help(const char**, size_t);

    public:
    bool isDaemonizing() const { return m_daemonize; }

    bool isVertSyncEnabled() const {
        return m_vert_sync_enabled;
    }

    uint32_t getFramerateLimit() const {
        return m_framerate_limit;
    }

   private:
    const char* m_program_name = "xpapers";
    const char* m_program_path = nullptr;
    bool m_daemonize = false;

    bool m_vert_sync_enabled = false;

    uint32_t m_framerate_limit = 60;

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
