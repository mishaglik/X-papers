#include "Label.hpp"
#include <Engine/WallpaperEngine/MetaUtils.hpp>
#include "Utilities/log.hpp"
#include "SpriteWidget.hpp"

static const xppr::meta::MetaFuction LabelMeths[] = {
    META_METHOD(Label, setText),
    META_METHOD(Label, setFont),
    META_METHOD(Label, setSize),
    META_METHOD(Label, setTextColor),
    META_METHOD(Label, setTextPos),
    {nullptr, nullptr, nullptr},
};

const xppr::meta::MetaType LabelType =
{
    .name = "label",
    .methods = LabelMeths,
    .members = nullptr,
    .dtor = nullptr
};


Label::Label() {
}

void Label::drawOn(xppr::RenderTexture& tex) const  {
    tex.draw(m_text);
}

void Label::setText(const char* text) {
    m_text.setString(text);
}

void Label::setFont(const char* fontname) {
    m_font.loadFromFile(fontname);
    m_text.setFont(m_font);
}

void Label::setTextColor(uint64_t color) {
    m_text.setFillColor(xppr::Color(static_cast<uint32_t>(color)));
}

void Label::setSize(uint64_t size) {
    m_text.setCharacterSize(size);
}

void Label::setTextPos(uint64_t x, uint64_t y) {
    m_text.setPosition(static_cast<float>(x), static_cast<float>(y));
}

