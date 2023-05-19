////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "VideoHandler.hpp"
#include <spdlog/spdlog.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <exception>
#include <iostream>
#include <sfeMovie/Movie.hpp>
#include <stdexcept>
#include "../WindowEngine.hpp"
#include "../XDisplayHandler.hpp"

namespace winengine {

VideoHandler::VideoHandler(const std::string& path, MonitorInfo& monitor)
    : m_video_path(path), m_renderer(), m_monitor_info(monitor) {
    if (path.empty()) {
        THROW(std::invalid_argument("empty video path"));
    }

    if (!m_renderer.openFromFile(m_video_path)) {
        std::cout << "Opened the file\n";
        THROW(std::runtime_error("can't open video file"));
    }
}

void VideoHandler::start(
    std::function<void(VideoHandler* video, sf::Event& ev)> func) {
// because xlib is NOT DOING #undef
#undef None
    auto m_monitor_dimensions = m_monitor_info.getDimensions();
    auto width = static_cast<unsigned int>(m_monitor_dimensions.x);
    auto height = static_cast<unsigned int>(m_monitor_dimensions.y);

    m_renderwindow = new sf::RenderWindow(sf::VideoMode(width - 1, height),
                                          "X-papers", sf::Style::None);

    auto m_monitor_positions = m_monitor_info.getPosition();
    auto x_pos = static_cast<int>(m_monitor_positions.x);
    auto y_pos = static_cast<int>(m_monitor_positions.y);

    m_renderwindow->setPosition({x_pos, y_pos});

    auto manager = winengine::XDisplayHandler::getInstance();
    m_xlib_window = manager->addWindow(m_renderwindow->getSystemHandle());

    auto wintype = manager->createAtom("_NET_WM_WINDOW_TYPE", False);
    auto desktop = manager->createAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);

    m_xlib_window->changeProperty(wintype, XA_ATOM, 32, PropModeReplace,
                                  (unsigned char*)&desktop, 1);

    unsigned int ints[2];
    ints[0] = 0xFFFFFFFF;
    ints[1] = 2;

    auto wm_desktop = manager->createAtom("_NET_WM_DESKTOP", False);

    m_xlib_window->changeProperty(wm_desktop, XA_ATOM, 32, PropModeReplace,
                                  (unsigned char*)ints, 2);

    m_renderwindow->setFramerateLimit(m_fps_limit);
    m_renderwindow->setVerticalSyncEnabled(m_vert_sync_enabled);

    m_renderer.fit(0, 0, static_cast<float>(width), static_cast<float>(height));
    m_renderer.play();

    while (m_renderwindow->isOpen()) {
        sf::Event ev;
        if (!(m_renderer.getStatus() == sfe::Playing)) {
            m_renderer.play();
        }
        while (m_renderwindow->pollEvent(ev)) {
            // handle events by user
            func(this, ev);
            /*
            if (ev.type == sf::Event::Closed ||
                (ev.type == sf::Event::KeyPressed &&
                 ev.key.code == sf::Keyboard::Escape)) {
                m_renderwindow->close();
            }

            if (ev.type == sf::Event::Resized) {
                m_renderer.fit(0, 0, m_renderwindow->getSize().x,
                                     m_renderwindow->getSize().y);
                m_renderwindow->setView(
                    sf::View(sf::FloatRect(0, 0,
            (float)m_renderwindow->getSize().x,
                                           (float)m_renderwindow->getSize().y)));
            }
            */
        }

        m_renderer.update();

        m_renderwindow->clear();
        m_renderwindow->draw(m_renderer);
        m_renderwindow->display();
    }

    manager->closeDisplay();
}

void VideoHandler::setFPSlimit(const uint32_t new_limit) { m_fps_limit = new_limit; }

void VideoHandler::enableVerticalSync() { m_vert_sync_enabled = true; }

bool VideoHandler::isOpen() const { return m_renderwindow->isOpen(); }

std::size_t VideoHandler::getFPSlimit() const { return m_fps_limit; }

std::string VideoHandler::getVideoPath() const { return m_video_path; }

bool VideoHandler::isVertSyncEnabled() const { return m_vert_sync_enabled; }

sf::RenderWindow* VideoHandler::getRenderWindow() { return m_renderwindow; }

XWindowHandler* VideoHandler::getHandler() { return m_xlib_window; }

}  // namespace winengine
