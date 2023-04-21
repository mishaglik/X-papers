#include <cassert>
#include <unistd.h>
#include <Utilities/defines.hpp>

int main() {
    xppr::log::init_logger();
    assert("True" != nullptr);
    //Test WF
}
