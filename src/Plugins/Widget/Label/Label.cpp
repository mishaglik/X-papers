#include "Label.hpp"
#include <Engine/WallpaperEngine/MetaDefaults.hpp>
static const xppr::meta::MetaFunction LabelMeths[] = {
    META_METHOD(Label, setText),
    META_METHOD(Label, setFont),
    META_METHOD(Label, setSize),
    META_METHOD(Label, setColor),
    META_METHOD(Label, setPos),
    {nullptr, nullptr, nullptr},
};

Label::Label() {
    m_name = "label";
    m_methods = LabelMeths;
    m_members = nullptr;
}


void Label::update(uint64_t)  {
}

void Label::draw(xppr::RenderWindow& window) const  {
    window.draw(m_text);
}

void Label::setText(const char* text) {
    m_text.setString(text);
}

void Label::setFont(const char* fontname) {
    m_font.loadFromFile(fontname);
    m_text.setFont(m_font);
}

void Label::setColor(uint64_t color) {
    m_text.setFillColor(xppr::Color(static_cast<uint32_t>(color)));
}

void Label::setSize(uint64_t size) {
    m_text.setCharacterSize(size);
}
void Label::setPos(uint64_t x, uint64_t y) {
    m_text.setPosition(static_cast<float>(x), static_cast<float>(y));
}

struct Lab : xppr::meta::MetaClass {
    xppr::ApplicationAPI api;
};

xppr::meta::MetaClass* createLabel(xppr::meta::ArgPack* ap) {
    auto lab = static_cast<Lab*>(ap->self);
    Label* label = new Label;
    lab->api.addWidget(label, 0);
    return label;
}

static const xppr::meta::MetaFunction LabAdd[] = {
    {"add", "L", createLabel},
    {nullptr, nullptr, nullptr},

};

extern "C" void init_plugin(xppr::ApplicationAPI api) { 
    static constexpr const char name[] = "label";
    static Lab lab = {{name, LabAdd, nullptr}, api};
    api.registerClass(&lab);
    
}
