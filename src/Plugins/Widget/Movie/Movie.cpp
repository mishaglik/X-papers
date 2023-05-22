#include "Movie.hpp"
#include <Engine/WallpaperEngine/MetaUtils.hpp>
#include "Engine/WinEngine/WindowEngine.hpp"
#include "Utilities/log.hpp"

static const xppr::meta::MetaFuction movieMethods[] = {
    META_METHOD(Movie, setVideoPath),
    META_METHOD(Movie, fit),
    {nullptr, nullptr, nullptr},
};

const xppr::meta::MetaType MovieType =
{
    .name = "movie",
    .methods = movieMethods,
    .members = nullptr,
    .dtor = nullptr
};


Movie::Movie() {
}

void Movie::draw(xppr::RenderWindow &window) const {
    window.draw(m_renderer);
}

void Movie::update(uint64_t) {
    if (!(m_renderer.getStatus() == sfe::Playing)) {
        m_renderer.play();
    }

    m_renderer.update();
}


void Movie::setVideoPath(const char *path) {
    if (path != nullptr) {
        if (!m_renderer.openFromFile(path)) {
            xppr::log::critical("Can't open file %s\n", path);
        }
    } else {
        xppr::log::error("Empty path is not allowed\n");
    }
}

void Movie::fit(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    m_renderer.fit(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
    m_renderer.play();
}
