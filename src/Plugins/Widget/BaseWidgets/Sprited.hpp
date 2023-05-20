#ifndef PLUGINS_WIDGET_BASEWIDGETS_SPRITED_HPP
#define PLUGINS_WIDGET_BASEWIDGETS_SPRITED_HPP
#include "Engine/WallpaperEngine/MetaTypes.hpp"
#include "Engine/WallpaperEngine/MetaUtils.hpp"
#include "SpriteWidget.hpp"
namespace sprited {

template<SpriteWidget Widget>
struct SWidget;

template<SpriteWidget Widget>
constexpr const std::array<xppr::meta::MetaFuction, 2> SWMeths = 
{
    xppr::meta::MetaFuction{"setPos", "LL", xppr::meta::detail::GetCallback<decltype(&SWidget<Widget>::setPosition)>::template call<&SWidget<Widget>::setPosition>},
    xppr::meta::MetaFuction{}
};

template<SpriteWidget Widget>
constexpr const auto SWMethsFull = xppr::meta::ZArrConcat(SWMeths<Widget>, Widget::m_meths);

template<SpriteWidget Widget>
constexpr const xppr::meta::MetaType SWMetaType = 
{
    .name = "SWidget",
    .methods = SWMethsFull<Widget>.data(),
    .members = nullptr,
    .dtor = nullptr
};

template<SpriteWidget Widget>
struct SWidget : Widget, xppr::WidgetBase {
    static constexpr const xppr::meta::MetaType* const Type = &SWMetaType<Widget>;

    SWidget(Widget wid, size_t w, size_t h) : Widget(std::move(wid)), xppr::WidgetBase() {
        Widget::MetaObject::m_type = Type;
        m_texture.create(w, h);
        m_sprite.setTexture(m_texture.getTexture());
        m_sprite.setOrigin(0, static_cast<float>(m_sprite.getTextureRect().height));
        m_sprite.setScale(1, -1);
    }
    
    virtual ~SWidget() override {}
    
    virtual void draw(xppr::RenderWindow& window) const override  {
        Widget::drawOn(m_texture);
        window.draw(m_sprite);
    }

    virtual void update(uint64_t curtime) override {
        Widget::update(curtime);
    }
    /**
     * @brief handle event
     * 
     * @return true - if event is consumed and must not be passed next;
     * @return false - else;
     */
    virtual bool handleEvent(xppr::EventBase&) override { return false; }
    
    virtual const char* getName() const override { return "SWidget"; }
    
    void setPosition(uint64_t x, uint64_t y) {
        m_sprite.setPosition(x, y);
    }

private:
    mutable xppr::RenderTexture m_texture;
    xppr::Sprite m_sprite;
};

struct Sprited {
    size_t w, h;

    template <SpriteWidget Widget>
    auto* Pipe(Widget widget) {
        return new SWidget<Widget>(std::move(widget), w, h);
    }
};

}

inline auto OnSprite(size_t w, size_t h) {
  return sprited::Sprited{w, h};
}

#endif /* PLUGINS_WIDGET_BASEWIDGETS_SPRITED_HPP */
