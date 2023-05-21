#ifndef ENGINE_WALLPAPERENGINE_WIDGET_HPP
#define ENGINE_WALLPAPERENGINE_WIDGET_HPP

#include <cstddef>
#include <cstdint>

#include <Engine/RenderEngine/REngine.hpp>

namespace xppr {
// class RenderWindow;
struct EventBase;
class WidgetBase
{
public:
    virtual ~WidgetBase();

    /**
     * @brief Draw widget on this widow.
     * 
     * @param window 
     */
    virtual void draw(RenderWindow& window) const = 0;
    
    /**
     * @brief Update function. 
     * 
     * @param curtime - used for animation sync. It's guaranted that curtime is same for everyone in single update cycle.
     */
    virtual void update(uint64_t curtime) = 0;

    /**
     * @brief handle event
     * 
     * @return true - if event is consumed and must not be passed next;
     * @return false - else;
     */
    virtual bool handleEvent(EventBase&) = 0;
    
    [[deprecated("Now useless")]]
    size_t m_display_idx = 0;

    [[deprecated("Naming moved to meta object system")]]
    virtual const char* getName() const;
};
}

#endif /* ENGINE_WALLPAPERENGINE_WIDGET_HPP */
