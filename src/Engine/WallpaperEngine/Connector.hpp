#ifndef ENGINE_WALLPAPERENGINE_CONNECTOR_HPP
#define ENGINE_WALLPAPERENGINE_CONNECTOR_HPP

/**
    This header is to be used as exported to write plugin. 
*/

#include <Engine/WallpaperEngine/Event.hpp>
#include <Engine/WallpaperEngine/Widget.hpp>
#include <Engine/WallpaperEngine/MetaObject.hpp>
namespace xppr {
    namespace wpeng {
        class WallpaperEngine;
    }
    class ConnectorBase;

    /**
     * @brief Must be sync with xppr::wpeng::WPError
     * 
     */
    enum APIError : uint64_t {
        OK = 0,
        Invalid,
        Unknown,
    };

    // Pimple to WallpaperEngine both uninifing api and hiding class. 
    class ApplicationAPI {
        ApplicationAPI(wpeng::WallpaperEngine* engine) : m_wallpaper(engine) {}
    public:
        APIError addWidget(WidgetBase* widget);
        APIError addConnector(ConnectorBase* connector);
        APIError loadPlugin(const char* path);
    private:
        friend class wpeng::WallpaperEngine;
        wpeng::WallpaperEngine* m_wallpaper;
    };

    /* Connector plugins must implement next methods 
        extern ConnectorBase* init_plugin(ApplicationAPI api);
    */

    class ConnectorBase {
    public:
        virtual void update(uint64_t curtime) = 0;
        virtual void widgetCreated(WidgetBase* widget) = 0;
        virtual ~ConnectorBase() = 0;
    };
}

#endif /* ENGINE_WALLPAPERENGINE_CONNECTOR_HPP */
