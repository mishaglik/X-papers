#ifndef PLUGINS_WIDGET_BASEWIDGETS_BORDER_HPP
#define PLUGINS_WIDGET_BASEWIDGETS_BORDER_HPP

#include "SpriteWidget.hpp"
#include <Engine/WallpaperEngine/MetaUtils.hpp>
#include <utility>

namespace decor {
    
template<SpriteWidget Widget>
struct BorderDecorator;


namespace detail {

template<SpriteWidget Widget>
constexpr const std::array<xppr::meta::MetaFuction, 2> BorderMeths = 
{
    xppr::meta::MetaFuction{"setBorderColor", "L", xppr::meta::detail::GetCallback<decltype(&BorderDecorator<Widget>::setBorderColor)>::template call<&BorderDecorator<Widget>::setBorderColor>},
    xppr::meta::MetaFuction{}
};

template<SpriteWidget Widget>
constexpr const auto BorderMethsFull = xppr::meta::ZArrConcat(BorderMeths<Widget>, Widget::m_meths);


}

template<SpriteWidget W>
struct BorderDecorator : public W {
    BorderDecorator(W w, uint32_t color) : W(std::move(w)), m_color(color) {}

    void setBorderColor(uint32_t color) {
        m_color = color;
    }

    void setBorderThikness(uint64_t width) {
        m_width = width;
    }

    void drawOn(xppr::RenderTexture& texture) const {
        W::drawOn(texture);
        xppr::RectangleShape rect;
        rect.setSize(xppr::Vector2f(texture.getSize().x, texture.getSize().y));
        rect.setFillColor(xppr::Color::Transparent);
        rect.setOutlineColor(xppr::Color(m_color));
        rect.setOutlineThickness(-static_cast<float>(m_width));
        texture.draw(rect);
    }

    constexpr static const auto m_meths = detail::BorderMethsFull<W>;
private:
    uint32_t m_color = xppr::Color::Black.toInteger();
    uint64_t m_width = 1;
};

namespace pipe {

struct [[nodiscard]] Bordered {
    uint32_t m_color;
    template <SpriteWidget Wid>
    SpriteWidget auto Pipe(Wid widget) {
        return BorderDecorator(std::move(widget), m_color);
    }
};
}
}

inline auto Bordered(uint32_t color) {
  return decor::pipe::Bordered{color};
}


#endif /* PLUGINS_WIDGET_BASEWIDGETS_BORDER_HPP */
