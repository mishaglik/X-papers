#include "Background.hpp"
#include <Engine/WallpaperEngine/MetaUtils.hpp>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace xppr::wpeng {

    static const meta::MetaFuction BgMeths[] = {
        META_METHOD(Background, addImage),
        {}
    };

    const meta::MetaType detail::BgType = {
        "bg",
        BgMeths,
        nullptr,
        nullptr
    };

    Background::Background(Vector2u /* winsize */) : Background() {
       
    }
    
    void Background::addImage(const char* filename) {
        xppr::Image image;
        if(!image.loadFromFile(filename)) {
            xppr::log::error("Failed to open file {}", filename);
            return;
        }
        m_images.emplace_back();
        m_images.back().loadFromImage(image);
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

    meta::MetaObject* BgMgr::add(uint64_t i) {
        auto* bg = new Background();
        m_api.addWidget(bg, i);
        return bg;
    }

   
    const meta::MetaFuction BgMgrMeths[] {
        META_METHOD(BgMgr, add),
        {nullptr, nullptr}
    };

    const meta::MetaType BgMgrType {
        .name = "bg",
        .methods = BgMgrMeths,
        .members = nullptr,
        .dtor = nullptr
    };
}
