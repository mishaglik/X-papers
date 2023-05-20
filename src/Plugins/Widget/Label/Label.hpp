#ifndef PLUGINS_WIDGET_LABEL_LABEL_HPP
#define PLUGINS_WIDGET_LABEL_LABEL_HPP

#include <Engine/WallpaperEngine/Connector.hpp>

extern "C" void init_plugin(xppr::ApplicationAPI api);

extern const xppr::meta::MetaType LabelType;

class Label : public xppr::WidgetBase, public xppr::meta::MetaObjectT<&LabelType> {
public:
    Label();
    const char* getName() const override {return "label";}
    void update(uint64_t curtime) override;
    void draw(xppr::RenderWindow& window) const override;
    bool handleEvent(xppr::EventBase&) override {return false;}
    void setText(const char* text);
    void setFont(const char* fontname);
    void setTextColor(uint64_t color);
    void setSize(uint64_t size);
    void setTextPos(uint64_t x, uint64_t y);
    xppr::Text m_text;
    xppr::Font m_font;
};

#endif /* PLUGINS_WIDGET_LABEL_LABEL_HPP */
