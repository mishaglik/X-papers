#include "Label.hpp"
#include <Engine/WallpaperEngine/MetaUtils.hpp>
#include "Utilities/log.hpp"

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

void Label::setTextColor(uint64_t color) {
    m_text.setFillColor(xppr::Color(static_cast<uint32_t>(color)));
}

void Label::setSize(uint64_t size) {
    m_text.setCharacterSize(size);
}

void Label::setTextPos(uint64_t x, uint64_t y) {
    m_text.setPosition(static_cast<float>(x), static_cast<float>(y));
}

struct BorderRTDecorator : xppr::WidgetBase {
    BorderRTDecorator(uint32_t color = 0xFF0F00FF) : m_color(color) {
        m_color = rand() & 0xFFFFFFFF;
        m_tex.create(200, 30);
    }

    void draw(xppr::RenderWindow& texture) const override {
        xppr::RectangleShape rect;
        rect.setSize(xppr::Vector2f(200, 30));
        rect.setFillColor(xppr::Color::Transparent);
        rect.setOutlineColor(xppr::Color(m_color));
        rect.setOutlineThickness(-static_cast<float>(m_width));
        m_tex.draw(rect);
        sf::Sprite sp(m_tex.getTexture());
        texture.draw(sp);
    }
    
    virtual void update(uint64_t curtime) override {
    }
    
    virtual bool handleEvent(xppr::EventBase&) override { return false; }

private:
    uint32_t m_color = xppr::Color::Black.toInteger();
    uint64_t m_width = 1;
    mutable xppr::RenderTexture m_tex;
};

extern const xppr::meta::MetaType LabelMgrType;

struct LabelMgr : xppr::meta::MetaObjectT<&LabelMgrType> {
    xppr::ApplicationAPI m_api;
    xppr::meta::MetaObject* add(uint64_t i);
};

xppr::meta::MetaObject* LabelMgr::add(uint64_t i) {
    if(i >= m_api.NDisplays()) {
        return new xppr::meta::MetaError{{}, xppr::meta::MetaError::Type::AttributeError, "index of display is too big!"};
    }
    auto* bg = new Label();
    auto* rect = new BorderRTDecorator();
    m_api.addWidget(rect, i);
    m_api.addWidget(bg, i);
    return bg;
}

static void
LabelMgrDtor(xppr::meta::MetaObject* obj) {
    delete static_cast<LabelMgr*>(obj);
}

const xppr::meta::MetaFuction LabelMgrMeths[] {
    META_METHOD(LabelMgr, add),
    {nullptr, nullptr, nullptr}
};

const xppr::meta::MetaType LabelMgrType {
    .name = "label",
    .methods = LabelMgrMeths,
    .members = nullptr, 
    .dtor = LabelMgrDtor
};

extern "C" void init_plugin(xppr::ApplicationAPI api) { 
    xppr::log::add_logger();
    api.registerObject(new LabelMgr{{}, api});
}
