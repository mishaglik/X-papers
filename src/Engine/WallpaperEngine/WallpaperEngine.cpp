#include "WallpaperEngine.hpp"
#include "WallpaperEngine/Background.hpp"
#include <time.h>
#include <cstddef>

namespace xppr {
using namespace wpeng;

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


}