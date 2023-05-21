#include "Configurator.hpp"

#include <cstring>

#include <Utilities/utils.hpp>

#define SET(x) &Configurator::GenericSet<&Configurator::x>

const Configurator::ParseEntry Configurator::CommandArguments[] = {
    {"--daemonize", "Run as daemon", SET(m_daemonize)},
    {"-d",          "Run as daemon", SET(m_daemonize)},
    {"--help",      "Print help", &Configurator::Help}
};

#undef SET

void Configurator::genConfig(int argc, const char* argv[]) {
    if(argc < 0) {
        xppr::log::error("argc is {}. Total cringe - using default config", argc);
        return;
    }

    if(argc == 0) {
        xppr::log::warn("argc is 0... Sus. Using default config");
        return;
    }

    m_program_path = argv[0];
    m_program_name = basename(argv[0]);

    size_t left = argc - 1;
    while(left) {
        size_t parsed = parseArgument(argv, left);
        if(parsed == -1ul) {
            Help(argv, argc);
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
    if(size == 0) {
        return 0;
    }

    for(size_t i = 0; i < SIZEOF_ARR(CommandArguments); ++i) {
        if(matches(args[0], CommandArguments[i].m_key)) {
            return (this->*CommandArguments[i].m_callback)(args, size);
        }
    }
    xppr::log::error("Argument {} does not match any of existing keys", args[0]);
    printf("Unkown argument: \"%s\".", args[0]);
    return -1;
}
