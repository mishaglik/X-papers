#ifndef ENGINE_RENDERENGINE_RENGINE_HPP
#define ENGINE_RENDERENGINE_RENGINE_HPP

#include <cstdint>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
namespace xppr {

    using namespace sf; //HACK //FIXME Temproary stub

    class Renderer : public sf::RenderWindow {
    public:
        using WindowHandle = uint64_t;
        
    };
}

#endif /* ENGINE_RENDERENGINE_RENGINE_HPP */
