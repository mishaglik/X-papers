#ifndef ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP
#define ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP
#include "WallpaperEngine/Widget.hpp"
#include <RenderEngine/REngine.hpp>
#include <Utilities/utils.hpp>
#include <chrono>
#include <cstddef>
namespace xppr {
    

    class WallpaperEngine {
      public:
        WallpaperEngine(const Vector<xppr::WindowHandle>& windows);

        WallpaperEngine(const WallpaperEngine&) = delete;
        WallpaperEngine& operator=(const WallpaperEngine&) = delete;
      
        ~WallpaperEngine();
        void run();

        void setBackgroundImages(const Vector<Image>& image, size_t win);
        

      private:
        Vector<RenderWindow> m_windows;
        Vector<IWidget* > m_widgets;
        bool m_quitted = false;
        void cycle();
        
        std::chrono::system_clock::time_point m_start;
    };

}

#endif /* ENGINE_WALLPAPERENGINE_WALLPAPERENGINE_HPP */
