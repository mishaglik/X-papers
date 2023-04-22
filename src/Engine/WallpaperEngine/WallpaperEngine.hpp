#ifndef ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP
#define ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP
#include "WallpaperEngine/Widget.hpp"
#include <RenderEngine/REngine.hpp>
#include <Utilities/utils.hpp>
namespace xppr {
    

    class WallpaperEngine {
      public:
        WallpaperEngine(const Vector<xppr::WindowHandle>& windows);

        WallpaperEngine(const WallpaperEngine&) = delete;
        WallpaperEngine& operator=(const WallpaperEngine&) = delete;
      
        void run();

      private:
        Vector<RenderWindow> m_windows;
        Vector<IWidget* > m_widgets;
        bool m_quitted = false;
        void cycle();
    };

}

#endif /* ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP */
