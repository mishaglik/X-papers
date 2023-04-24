#include <cassert>
#include <unistd.h>
#include <Utilities/defines.hpp>
#include <Engine/WinEngine/engine.hpp>

int main() {
    xppr::log::init_logger();
    
    test_open_window();
}
