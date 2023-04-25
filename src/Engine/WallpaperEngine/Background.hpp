#ifndef ENGINE_WALLPAPERENGINE_BACKGROUND_HPP
#define ENGINE_WALLPAPERENGINE_BACKGROUND_HPP

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <WallpaperEngine/Widget.hpp>
#include <Utilities/utils.hpp>
#include <cstddef>
#include <cstdint>
namespace xppr::wpeng {

class Background : public WidgetBase {
public:
    explicit Background(Vector2u winsize);

    const char* getName() const override {return "background";}
    void update(uint64_t curtime) override;
    void draw(RenderWindow& window) const override;
    bool handleEvent(EventBase&) override {return false;}

    void setImageList(const Vector<Image>& images);
private:
    Vector<Texture> m_images;
    uint64_t m_update_speed = 1'000;
    Sprite m_sprite;
};

}

#endif /* ENGINE_WALLPAPERENGINE_BACKGROUND_HPP */
