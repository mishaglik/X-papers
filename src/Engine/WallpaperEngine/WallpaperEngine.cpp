#include "WallpaperEngine.hpp"

#include <dlfcn.h>
#include <time.h>
#include <cstddef>

#include <WallpaperEngine/Background.hpp>
#include <WallpaperEngine/Connector.hpp>

namespace xppr::wpeng {

WallpaperEngine::WallpaperEngine(const Vector<RenderWindow*>& windows)
    : m_displays(windows.size()) {
    for (size_t i = 0; i < windows.size(); ++i) {
        m_displays[i].renderer = windows[i];
        m_displays[i].widgets.push_back(
            new Background(m_displays[i].renderer->getSize()));
    }
}

void WallpaperEngine::run() {
    while (!m_asked_quit) {
        cycle();
    }
}

void WallpaperEngine::cycle() {
    timeval cur_timeval;  // TODO: Change to std::chorno.
    gettimeofday(&cur_timeval, nullptr);
    uint64_t curtime = cur_timeval.tv_sec * 1000 + cur_timeval.tv_usec / 1000;

    for (Display& display : m_displays) {
        Event event;
        while (display.renderer->pollEvent(event)) {
            // FIXME: Proper event fixing.
            if (event.type == Event::EventType::Closed) {
                quit();
            }
        }

        for (WidgetBase* widget : display.widgets) {
            widget->update(curtime);
        }

        for (WidgetBase* widget : display.widgets) {
            widget->draw(*display.renderer);
        }

        display.renderer->display();
    }
}

WallpaperEngine::~WallpaperEngine() {
    for(auto* connector : m_connectors) {
        delete connector;
    }
}

WPError WallpaperEngine::loadPlugin(const char* filename) {
    if (strcmp(filename, "bg") == 0) {
        registerObject(new BgMgr{{}, ApplicationAPI(this)});
        return Ok;
    }

    void* handle = dlopen(filename, RTLD_NOW | RTLD_LOCAL);  // TODO: dlclose
    if (!handle) {
        xppr::log::error("loadPlugin \"{}\" failed with error: {}", filename,
                         dlerror());
        return WPError::Invalid;
    }

    using Initer = void (*)(ApplicationAPI);

    Initer initer = reinterpret_cast<Initer>(dlsym(handle, "init_plugin"));
    if (!initer) {
        xppr::log::error("loadPlugin \"{}\" failed with error: {}", filename,
                         dlerror());
        dlclose(handle);
        return WPError::Invalid;
    }

    initer(ApplicationAPI(this));
    return WPError::Ok;
}

WPError WallpaperEngine::addConnector(ConnectorBase* connector) {
    m_connectors.push_back(connector);
    return Ok;
}

WPError WallpaperEngine::registerClass(xppr::meta::MetaType* meta) {
    for (ConnectorBase* cb : m_connectors) {
        cb->registerClass(meta);
    }
    return Ok;
}

WPError WallpaperEngine::registerObject(xppr::meta::MetaObject* meta) {
    for (ConnectorBase* cb : m_connectors) {
        cb->registerObject(meta);
    }
    return Ok;
}

WPError WallpaperEngine::addWidget(WidgetBase* widget, uint64_t display) {
    if (display > m_displays.size()) {
        return WPError::Invalid;
    }
    m_displays[display].widgets.push_back(widget);
    return Ok;
}

}  // namespace xppr::wpeng

namespace xppr::meta {
// TODO: Move from here.
const MetaType ErrorType = {"error", nullptr, nullptr, nullptr};

}  // namespace xppr::meta
