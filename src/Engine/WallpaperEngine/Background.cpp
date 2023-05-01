#include "Background.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace xppr::wpeng {

    Background::Background(Vector2u winsize) : m_meta(this) {
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

    meta::MetaClass* setImage(meta::ArgPack* ap) {
        Image img;
        img.loadFromFile(reinterpret_cast<char*>(ap->m_data[0]));
        reinterpret_cast<Background::BgMeta*>(ap->self)->m_bg->setImageList({img});
        return nullptr;
    }

    static meta::MetaFunction BgMeths[] = {
        {"setImage", "s", setImage},
        {NULL}
    };

    Background::BgMeta::BgMeta(Background* bg) : m_bg(bg) {
        m_methods = BgMeths;
        m_members = 0;
        m_name = "bg";
    }
    
}
