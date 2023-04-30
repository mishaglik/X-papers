#include "WallpaperEngine.hpp"
#include "WallpaperEngine/Background.hpp"
#include "WallpaperEngine/Connector.hpp"
#include <time.h>
#include <cstddef>
#include <dlfcn.h>
namespace xppr::wpeng {

WallpaperEngine::WallpaperEngine(const Vector<XWindowHandler* >& windows) : m_displays(windows.size())
{
    for(size_t i = 0; i < windows.size(); ++i) {
        m_displays[i].xhandler = windows[i];
        m_displays[i].renderer.create(windows[i]->getInternalHandler());
        m_displays[i].widgets.push_back(new Background(m_displays[i].renderer.getSize()));
    }
}

void WallpaperEngine::run() {
    while(!m_quitted) {
        cycle();
    }
}

void WallpaperEngine::cycle() {

    timeval cur_timeval;
    gettimeofday(&cur_timeval, NULL);
    uint64_t curtime = cur_timeval.tv_sec * 1000 + cur_timeval.tv_usec / 1000;

    // Manage events
    for(Display& display : m_displays) {

        for(WidgetBase* widget : display.widgets){
            widget->update(curtime);
        }

        for(WidgetBase* widget : display.widgets){
            widget->draw(display.renderer);
        }

        display.renderer.display();
    }

}

void WallpaperEngine::setBackgroundImages(const Vector<Image>& image, size_t win)
{
    auto bg = static_cast<Background*>(m_displays[win].widgets[0]);
    bg->setImageList(image);
}

WallpaperEngine::~WallpaperEngine() {}

WPError WallpaperEngine::loadPlugin(const char* filename) {
    void* handle = dlopen(filename, RTLD_NOW | RTLD_LOCAL); //TODO: dlclose
    if(!handle) {
        xppr::log::error("loadPlugin \"{}\" failed with error: {}", filename, dlerror());
        return WPError::Invalid;
    }

    using Initer = void(*)(ApplicationAPI);

    Initer initer = reinterpret_cast<Initer>(dlsym(handle, "init_plugin"));
    if(!initer) {
        xppr::log::error("loadPlugin \"{}\" failed with error: {}", filename, dlerror());
        dlclose(handle);
        return WPError::Invalid;
    }

    initer(ApplicationAPI(this));
    return WPError::Ok;
}

}