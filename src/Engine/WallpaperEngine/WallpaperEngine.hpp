#ifndef ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP
#define ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP

#include <csignal>
#include <cstddef>

#include <RenderEngine/REngine.hpp>

#include <WallpaperEngine/Connector.hpp>
#include <WallpaperEngine/Widget.hpp>

#include <Utilities/utils.hpp>


namespace xppr::wpeng {

enum WPError : uint64_t {
    Ok,
    Invalid,
    Unknown,
};

class WallpaperEngine {

   public:
    WallpaperEngine(const Vector<RenderWindow* >& windows);

    WallpaperEngine(const WallpaperEngine&) = delete;

    WallpaperEngine& operator=(const WallpaperEngine&) = delete;

    ~WallpaperEngine();
    
    void run();

    void quit() { m_asked_quit = 1; }

    WPError loadPlugin(const char* filename);

    WPError addWidget(WidgetBase* widget, uint64_t display = 0);

    WPError addConnector(ConnectorBase* connector);

    WPError registerClass(xppr::meta::MetaType* meta);

    WPError registerObject(xppr::meta::MetaObject* meta);

    size_t GetNDisplays() const { return m_displays.size(); }

   private:

    struct Display {
        RenderWindow* renderer;
   
        Vector<WidgetBase*> widgets;
   
        Display() = default;
   
        Display(const Display&) = delete;
   
        Display& operator=(const Display&) = delete;
   
        ~Display() {
            for (WidgetBase* widget : widgets)
                delete widget;
        }
    };

    Vector<ConnectorBase*> m_connectors;

    Vector<Display> m_displays;
    
    void cycle();

    volatile std::sig_atomic_t m_asked_quit = 0;
};

}  // namespace xppr::wpeng

#endif /* ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP */
