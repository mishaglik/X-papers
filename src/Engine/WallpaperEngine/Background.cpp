#include "Background.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace xppr::wpeng {

     static meta::MetaClass* setImage(meta::ArgPack* ap) {
        Image img;
        if(!img.loadFromFile(reinterpret_cast<char*>(ap->m_data[0]))) {
            log::critical("Img not loaded");
            return nullptr;
        }
        
        assert(ap->self);
        static_cast<Background* >(ap->self)->setImageList({img});
        return nullptr;
    }

    static meta::MetaFunction BgMeths[] = {
        {"setImage", "s", setImage},
        {NULL}
    };

    Background::Background() {
        m_name = "bg";
        m_methods = BgMeths;
        m_members = nullptr;
    }

    Background::Background(Vector2u /* winsize */) : Background() {
       
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
