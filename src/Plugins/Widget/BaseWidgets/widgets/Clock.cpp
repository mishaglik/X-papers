#include "Clock.hpp"
#include <Engine/WallpaperEngine/MetaUtils.hpp>
#include "Utilities/log.hpp"
#include "SpriteWidget.hpp"
#include <time.h>

static const xppr::meta::MetaFuction ClockMeths[] = {
    META_METHOD(Clock, setFont),
    META_METHOD(Clock, setTextSize),
    META_METHOD(Clock, setTextColor),
    META_METHOD(Clock, setTextPos),
    {nullptr, nullptr, nullptr},
};

const xppr::meta::MetaType ClockType =
{
    .name = "Clock",
    .methods = ClockMeths,
    .members = nullptr,
    .dtor = nullptr
};


Clock::Clock() {
}

void Clock::drawOn(xppr::RenderTexture& tex) const  {
    tex.draw(m_text);
}

void Clock::update(uint64_t) {
    struct tm* tm_info;
    time_t timer = time(NULL);
    tm_info = localtime(&timer);
    strftime(m_timebuf, 50, "%H:%M:%S", tm_info);
    m_text.setString(m_timebuf);
}


void Clock::setFont(const char* fontname) {
    m_font.loadFromFile(fontname);
    m_text.setFont(m_font);
}

void Clock::setTextColor(uint64_t color) {
    m_text.setFillColor(xppr::Color(static_cast<uint32_t>(color)));
}

void Clock::setTextSize(uint64_t size) {
    m_text.setCharacterSize(size);
}

void Clock::setTextPos(uint64_t x, uint64_t y) {
    m_text.setPosition(static_cast<float>(x), static_cast<float>(y));
}

