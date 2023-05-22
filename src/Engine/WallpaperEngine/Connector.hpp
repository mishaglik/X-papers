#ifndef ENGINE_WALLPAPERENGINE_CONNECTOR_HPP
#define ENGINE_WALLPAPERENGINE_CONNECTOR_HPP

/**
    This header is to be used as exported to write plugin.
*/

#include <Engine/WallpaperEngine/MetaTypes.hpp>
#include <Engine/WallpaperEngine/Widget.hpp>
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
    constexpr ApplicationAPI(wpeng::WallpaperEngine* engine)
        : m_wallpaper(engine) {}
    APIError addWidget(WidgetBase* widget, uint64_t display);
    APIError addConnector(ConnectorBase* connector);
    APIError loadPlugin(const char* path);
    APIError registerClass(xppr::meta::MetaType* meta);
    APIError registerObject(xppr::meta::MetaObject* meta);
    size_t NDisplays();

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

    virtual ~ConnectorBase();

    /**
     * @brief Update function. 
     * 
     * @param curtime - used for animation sync. It's guaranted that curtime is same for everyone in single update cycle.
     */
    virtual void update(uint64_t curtime) = 0;

    /**
     * @brief Register new metatype. Supposed to call before usage of any of objects with this type.
     * 
     * @param type - type to register. //TODO: return error;
     */
    virtual void registerClass(meta::MetaType* type) = 0;
    
    /**
     * @brief Register new object. It means that now main object will this object as member.
     *                      ^
     * @param meta_object  /
     */
    virtual void registerObject(meta::MetaObject* meta_object) = 0;

};
}  // namespace xppr

#endif /* ENGINE_WALLPAPERENGINE_CONNECTOR_HPP */
