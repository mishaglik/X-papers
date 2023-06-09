#ifndef ENGINE_WALLPAPERENGINE_BACKGROUND_HPP
#define ENGINE_WALLPAPERENGINE_BACKGROUND_HPP

#include <cstddef>
#include <cstdint>

#include <Engine/RenderEngine/REngine.hpp>
#include <WallpaperEngine/Connector.hpp>
#include <WallpaperEngine/Widget.hpp>
#include <Utilities/utils.hpp>

namespace xppr::wpeng {

namespace detail {
    extern const meta::MetaType BgType;
}

class Background : public WidgetBase,
                   public meta::MetaObjectT<&detail::BgType> {
   public:
    explicit Background(Vector2u winsize);
    explicit Background() = default;
    ~Background() override {}

    const char* getName() const override { return "background"; }
    
    void update(uint64_t curtime) override;
    
    void draw(RenderWindow& window) const override;
    
    bool handleEvent(EventBase&) override { return false; }

    void setImageList(const Vector<Image>& images);
    void addImage(const char* filename);
    void addImages(std::vector<std::string> filenames);

   private:
    Vector<Texture> m_images;
    uint64_t m_update_speed = 10'000;
    mutable Sprite m_sprite;
};

extern const meta::MetaType BgMgrType;

struct BgMgr : meta::MetaObjectT<&BgMgrType> {
    ApplicationAPI m_api;
    meta::MetaObject* add(uint64_t i);
};

}  // namespace xppr::wpeng

#endif /* ENGINE_WALLPAPERENGINE_BACKGROUND_HPP */
