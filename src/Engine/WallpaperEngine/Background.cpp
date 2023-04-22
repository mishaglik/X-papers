#include "Background.hpp"
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace xppr::wpeng {

    Background::Background(Vector2u winsize) {
    }

    void Background::update(uint64_t curtime) {
        if(m_images.empty()) [[unlikely]] {return;}
        size_t current = curtime / m_update_speed % m_images.size();
        m_sprite.setTexture(m_images[current]);
    }

    void Background::draw(RenderWindow& window) const {
        if(m_images.empty()) [[unlikely]] {
            window.clear();
        }
        window.draw(m_sprite);
    }

    void Background::setImageList(const Vector<Image>& images) {
        assert(!images.empty());
        m_images.resize(images.size());
        for(size_t i = 0; i < images.size(); ++i) {
            m_images[i].loadFromImage(images[i]);
        }
        m_sprite.setTexture(m_images[0]);
    }
}