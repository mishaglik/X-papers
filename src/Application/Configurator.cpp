#include "Configurator.hpp"

#include <csignal>
#include <cstring>
#include <unistd.h>

#include <Utilities/utils.hpp>

#define SET_(x) &Configurator::genericSet<&Configurator::x>

const Configurator::ParseEntry Configurator::CommandArguments[] = {
    {"--daemonize", "Run as daemon", SET_(m_daemonize)},
    {"-d",          "Run as daemon", SET_(m_daemonize)},
    {"--fps",          "Set FPS limit", SET_(m_framerate_limit)},
    {"--vs",          "Enable vertical sync", SET_(m_vert_sync_enabled)},
    {"--help",      "Print help", &Configurator::help}
};

#undef SET_

void Configurator::genConfig(int argc, const char* argv[]) {
    if (argc < 0) {
        xppr::log::error("argc is {}. Total cringe - using default config",
                         argc);
        return;
    }

    if (argc == 0) {
        xppr::log::warn("argc is 0... Sus. Using default config");
        return;
    }

    m_program_path = argv[0];
    m_program_name = basename(argv[0]);
    argv++;
    
    size_t left = argc - 1;
    while (left) {
        size_t parsed = parseArgument(argv, left);
        if (parsed == -1ul) {
            help(argv, argc);
            return;
        }
        left -= parsed;
        argv += parsed;
    }
}

static bool matches(const char* command, const char* key) {
    return strcmp(command, key) == 0;
}

size_t Configurator::parseArgument(const char* args[], size_t size) {
    if (size == 0) {
        return 0;
    }

    for (size_t i = 0; i < SIZEOF_ARR(CommandArguments); ++i) {
        if (matches(args[0], CommandArguments[i].m_key)) {
            return (this->*CommandArguments[i].m_callback)(args, size);
        }
    }
    xppr::log::error("Argument {} does not match any of existing keys",
                     args[0]);
    printf("Unkown argument: \"%s\".", args[0]);
    return -1;
}

size_t Configurator::help(const char**, size_t) {
    fmt::print("");
    return 1;
}

void daemonize() {
    if (daemon(1, 1) < 0) {
        xppr::log::critical("Daemonization finished with error: {}",
                            strerror(errno));
        exit(-1);
    }
}

static void (*ExHandler)(int) = nullptr;

static xppr::wpeng::WallpaperEngine* App = nullptr;

static void signalDispatch(int signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        static size_t attempt = 0;
        if (++attempt == 10) {
            exit(-1);
        }
        if (App) {
            App->quit();
        }
    }
}

void setupSignals(xppr::wpeng::WallpaperEngine* app) {
    App = app;
    ExHandler = signal(SIGTERM, signalDispatch);
    if (ExHandler == SIG_ERR) {
        xppr::log::critical("Unable to setup signal handler to SIGTERM");
        exit(-1);
    }

    ExHandler = signal(SIGINT, signalDispatch);
    if (ExHandler == SIG_ERR) {
        xppr::log::critical("Unable to setup signal handler to SIGINT");
        exit(-1);
    }
}
