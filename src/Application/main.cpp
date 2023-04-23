#include <cassert>
#include <unistd.h>
#include <Utilities/defines.hpp>
#include <Engine/WinEngine/XManager.hpp>


using winengine::XWindowManager;

int main() {
    xppr::log::init_logger();
    assert("True" != nullptr);
}
