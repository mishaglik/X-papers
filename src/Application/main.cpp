#include <Utilities/defines.hpp>
#include <Engine/WinEngine/XManager.hpp>


using winengine::XWindowManager;

int main() {
    xppr::log::init_logger();

    XWindowManager::getInstance().addWindow(0, 0, 150, 150, 10, nullptr);
}
