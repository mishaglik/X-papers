#include "WallpaperEngine.hpp"
#include "WallpaperEngine/Background.hpp"
#include <bits/chrono.h>
#include <chrono>
#include <cstddef>

namespace xppr {
using namespace wpeng;

WallpaperEngine::WallpaperEngine(const Vector<xppr::WindowHandle>& windows) : m_windows(windows.size()) {
    for(size_t i = 0; i < windows.size(); ++i) {
        m_windows[i].create(windows[i]);
        m_widgets.push_back(new Background(m_windows[i].getSize()));
        m_widgets.back()->m_drawOn = i;
    }

    m_start = std::chrono::system_clock::now();
}

void WallpaperEngine::run() {
    while(!m_quitted) {
        cycle();
    }
}

void WallpaperEngine::cycle() {

    uint64_t curtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_start).count();

    // Manage events

    for(auto widget : m_widgets) {
        widget->update(curtime);
    }

    for(auto widget : m_widgets) {
        widget->draw(m_windows[widget->m_drawOn]);
    }

    for(auto& window : m_windows) {
        window.display();
    }
}

void WallpaperEngine::setBackgroundImages(const Vector<Image>& image, size_t win)
{
    auto bg = static_cast<Background*>(m_widgets[win]);
    bg->setImageList(image);
}

WallpaperEngine::~WallpaperEngine() {
    for(auto widget: m_widgets) {
        delete widget;
    }
}


}