#ifndef ENGINE_WALLPAPERENGINE_CONNECTOR_HPP
#define ENGINE_WALLPAPERENGINE_CONNECTOR_HPP

/**
    This header is to be used as exported to write plugin. 
*/

#include <Engine/WallpaperEngine/Event.hpp>
#include <Engine/WallpaperEngine/Widget.hpp>
#include <Engine/WallpaperEngine/MetaTypes.hpp>
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
    public:
        constexpr ApplicationAPI(wpeng::WallpaperEngine* engine) : m_wallpaper(engine) {}
        APIError addWidget(WidgetBase* widget, uint64_t display);
        APIError addConnector(ConnectorBase* connector);
        APIError loadPlugin(const char* path);
        APIError registerClass(xppr::meta::MetaType* meta);
        APIError registerObject(xppr::meta::MetaObject* meta);
    private:
        friend class wpeng::WallpaperEngine;
        wpeng::WallpaperEngine* m_wallpaper;
    };

    /* Connector plugins must implement next methods 
        extern void init_plugin(ApplicationAPI api);
    */

    class ConnectorBase {
    public:
        ConnectorBase() = default;
        virtual void update(uint64_t curtime) = 0;
        virtual void registerClass(meta::MetaType* meta) = 0;
        virtual void registerObject(meta::MetaObject* meta) = 0;
        virtual ~ConnectorBase();
    };
}

#endif /* ENGINE_WALLPAPERENGINE_CONNECTOR_HPP */
