#include <cassert>
#include <unistd.h>
#include <Utilities/defines.hpp>
#include <Engine/WinEngine/engine.hpp>

int main() {
    xppr::log::init_logger();
    
    //test_open_window();
    test_video_handler("/home/execorn/Downloads/wallpaper.mp4");
    //test_gl("/home/execorn/Downloads/wallpaper.mp4");
    //test_render_wallpaper("/home/execorn/Downloads/wallpaper.mp4");
}
