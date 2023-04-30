#ifndef ENGINE_WALLPAPERENGINE_WIDGET_HPP
#define ENGINE_WALLPAPERENGINE_WIDGET_HPP

#include "Engine/WallpaperEngine/Event.hpp"
#include <cstddef>
#include <cstdint>

namespace xppr {
class RenderWindow;
class WidgetBase 
{
public:
    virtual ~WidgetBase() {}
    virtual void draw(RenderWindow& window) const = 0;
    virtual void update(uint64_t /*curtime*/) = 0;
    /**
     * @brief handle event
     * 
     * @return true - if event is consumed and must not be passed next;
     * @return false - else;
     */
    virtual bool handleEvent(EventBase&) = 0;
    
    size_t m_display_idx = 0;

    virtual const char* getName() const = 0;
};
}

#endif /* ENGINE_WALLPAPERENGINE_WIDGET_HPP */
