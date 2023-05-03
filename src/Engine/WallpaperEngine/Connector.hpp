#ifndef ENGINE_WALLPAPERENGINE_CONNECTOR_HPP
#define ENGINE_WALLPAPERENGINE_CONNECTOR_HPP

/**
    This header is to be used as exported to write plugin. 
*/

#include <WallpaperEngine/Event.hpp>
#include <WallpaperEngine/Widget.hpp>
namespace xppr::wpeng {
    class WallpaperEngine;
    class ConnectorBase;

    enum APIError {
        OK = 0,
        Invalid,
    };

    // Pimple to WallpaperEngine both uninifing api and hiding class. 
    class ApplicationAPI {
        ApplicationAPI(WallpaperEngine& engine) : m_wallpaper(engine) {}

        APIError addWidget(WidgetBase* widget);
        APIError addConnector(ConnectorBase* connector);
        APIError loadPlugin(const char* path);
    private:
        WallpaperEngine& m_wallpaper;
    };

    /* Connector plugins must implement next methods 
        extern void init_plugin(ApplicationAPI* api);
    */

    class ConnectorBase {
        virtual void update(uint64_t curtime) = 0;
        virtual void widgetCreated(WidgetBase* widget) = 0;
        virtual ~ConnectorBase() = 0;
    };
}

#endif /* ENGINE_WALLPAPERENGINE_CONNECTOR_HPP */
