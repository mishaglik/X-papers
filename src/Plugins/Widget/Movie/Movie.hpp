#ifndef XPAPER_PLUGINS_WIDGET_MOVIE_HPP
#define XPAPER_PLUGINS_WIDGET_MOVIE_HPP

#include <Engine/WallpaperEngine/Connector.hpp>
#include <sfeMovie/Movie.hpp>
#include <Engine/WinEngine/XWindowHandler.hpp>
#include <Engine/WinEngine/WindowEngine.hpp>

extern "C" void init_plugin(xppr::ApplicationAPI api);

extern const xppr::meta::MetaType MovieType;

class Movie : public xppr::WidgetBase, public xppr::meta::MetaObjectT<&MovieType> {
public:
    Movie();

    const char* getName() const override {return "movie";}

    void update(uint64_t curtime) override;
    
    void draw(xppr::RenderWindow& window) const override;
    
    bool handleEvent(xppr::EventBase&) override {return false;}
    
    void setVideoPath(const char* path);

    void fit(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

private:
    sfe::Movie m_renderer;
};

#endif // !XPAPER_PLUGINS_WIDGET_MOVIE_HPP
