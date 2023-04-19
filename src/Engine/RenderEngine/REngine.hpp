#ifndef ENGINE_RENDERENGINE_RENGINE_HPP
#define ENGINE_RENDERENGINE_RENGINE_HPP

#include <cstdint>
#include <SFML/Graphics/RenderWindow.hpp>
namespace xppr {
    class Renderer : public sf::RenderWindow {
    public:
        using WindowHandle = uint64_t;
        
    };
}

#endif /* ENGINE_RENDERENGINE_RENGINE_HPP */
