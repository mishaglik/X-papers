#include "WallpaperEngine.hpp"
#include "WallpaperEngine/Background.hpp"
#include <cstddef>

namespace xppr {
using namespace wpeng;

WallpaperEngine::WallpaperEngine(const Vector<xppr::WindowHandle>& windows) : m_windows(windows.size()) {
    for(size_t i = 0; i < windows.size(); ++i) {
        m_windows[i].create(windows[i]);
        m_widgets.push_back(new Background(m_windows[i].getSize()));
        m_widgets.back()->m_drawOn = i;
    }

}

void WallpaperEngine::run() {
    while(!m_quitted) {
        cycle();
    }
}

void WallpaperEngine::cycle() {

    // Manage events

    for(auto widget : m_widgets) {
        widget->update(0);
    }

    for(auto widget : m_widgets) {
        widget->draw(m_windows[widget->m_drawOn]);
    }
}


}