#ifndef ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP
#define ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP
#include "WallpaperEngine/Widget.hpp"
#include <RenderEngine/REngine.hpp>
#include "WallpaperEngine/Connector.hpp"
#include <WinEngine/XWindowHandler.hpp>
#include <Utilities/utils.hpp>
#include <cstddef>
namespace xppr::wpeng {    

    enum WPError : uint64_t {
      Ok,
      Invalid,
      Unknown,
    };


    class WallpaperEngine {
      using XWindowHandler = winengine::XWindowHandler;
      public:
        WallpaperEngine(const Vector<XWindowHandler* >& windows);

        WallpaperEngine(const WallpaperEngine&) = delete;
        WallpaperEngine& operator=(const WallpaperEngine&) = delete;
      
        ~WallpaperEngine();
        void run();

        void setBackgroundImages(const Vector<Image>& image, size_t win);
        

        WPError loadPlugin(const char* filename);
        WPError addWidget(WidgetBase* widget);
        WPError addConnector(ConnectorBase* connector);

      private:

      struct Display {
        XWindowHandler* xhandler;
        RenderWindow renderer;
        Vector<WidgetBase*> widgets;
        Display() = default;
        Display(const Display&) = delete;
        Display& operator=(const Display&) = delete;
        ~Display(){
          for(WidgetBase* widget : widgets) delete widget;
        }
      };

        Vector<ConnectorBase*> m_connectors;
        Vector<Display> m_displays;
        bool m_quitted = false;
        void cycle();
        
    };

}

#endif /* ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP */
