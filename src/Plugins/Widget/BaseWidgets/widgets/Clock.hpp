#ifndef PLUGINS_WIDGET_BASEWIDGETS_WIDGETS_CLOCK_HPP
#define PLUGINS_WIDGET_BASEWIDGETS_WIDGETS_CLOCK_HPP

#include <Engine/WallpaperEngine/Connector.hpp>
#include <Engine/WallpaperEngine/MetaUtils.hpp>

class Clock : public xppr::meta::MetaObject {
public:
    Clock();
    void update(uint64_t);
    void drawOn(xppr::RenderTexture& window) const;
    void setFont(const char* fontname);
    void setTextColor(uint64_t color);
    void setTextSize(uint64_t size);
    void setTextPos(uint64_t x, uint64_t y);
    xppr::Text m_text;
    xppr::Font m_font;
    static const std::array<xppr::meta::MetaFuction, 6> m_meths;
    char m_timebuf[50] = {};
};

constexpr const std::array<xppr::meta::MetaFuction, 6> Clock::m_meths =
{
    xppr::meta::MetaFuction META_METHOD(Clock, setFont),
    xppr::meta::MetaFuction META_METHOD(Clock, setTextSize),
    xppr::meta::MetaFuction META_METHOD(Clock, setTextColor),
    xppr::meta::MetaFuction META_METHOD(Clock, setTextPos),
    {nullptr, nullptr, nullptr},
};

#endif /* PLUGINS_WIDGET_BASEWIDGETS_WIDGETS_CLOCK_HPP */
