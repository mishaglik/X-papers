#ifndef ENGINE_WALLPAPERENGINE_BACKGROUND_HPP
#define ENGINE_WALLPAPERENGINE_BACKGROUND_HPP

#include <Engine/RenderEngine/REngine.hpp>
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
    virtual meta::MetaClass* getMeta() override {return &m_meta;}

    void setImageList(const Vector<Image>& images);
    struct BgMeta : meta::MetaClass {
        explicit BgMeta(Background* bg);
        Background* m_bg;
    };
private:
    Vector<Texture> m_images;
    uint64_t m_update_speed = 1'000;
    Sprite m_sprite;
    BgMeta m_meta;
    
};

}

#endif /* ENGINE_WALLPAPERENGINE_BACKGROUND_HPP */
