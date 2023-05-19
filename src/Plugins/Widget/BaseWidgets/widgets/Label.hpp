#ifndef PLUGINS_WIDGET_BASEWIDGETS_WIDGETS_LABEL_HPP
#define PLUGINS_WIDGET_BASEWIDGETS_WIDGETS_LABEL_HPP

#include <Engine/WallpaperEngine/Connector.hpp>
#include <Engine/WallpaperEngine/MetaUtils.hpp>

class Label : public xppr::meta::MetaObject {
public:
    Label();
    void update(uint64_t) {}
    void drawOn(xppr::RenderTexture& window) const;
    void setText(const char* text);
    void setFont(const char* fontname);
    void setTextColor(uint64_t color);
    void setSize(uint64_t size);
    void setTextPos(uint64_t x, uint64_t y);
    xppr::Text m_text;
    xppr::Font m_font;
    static const std::array<xppr::meta::MetaFuction, 6> m_meths;
};

constexpr const std::array<xppr::meta::MetaFuction, 6> Label::m_meths =
{
    xppr::meta::MetaFuction META_METHOD(Label, setText),
    xppr::meta::MetaFuction META_METHOD(Label, setFont),
    xppr::meta::MetaFuction META_METHOD(Label, setSize),
    xppr::meta::MetaFuction META_METHOD(Label, setTextColor),
    xppr::meta::MetaFuction META_METHOD(Label, setTextPos),
    {nullptr, nullptr, nullptr},
};

#endif /* PLUGINS_WIDGET_BASEWIDGETS_WIDGETS_LABEL_HPP */
