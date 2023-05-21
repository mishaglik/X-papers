////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "VideoHandler.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <Utilities/log.hpp>
#include <exception>
#include <iostream>
#include <memory>

#include <sfeMovie/Movie.hpp>
#include <stdexcept>
#include "WinEngine/WindowEngine.hpp"

namespace winengine {

VideoHandler::VideoHandler(const std::string& path, MonitorInfo& monitor, sf::RenderWindow* win)
    : m_video_path(path), m_renderer(), m_monitor_info(monitor), m_renderwindow(win) {
    if (path.empty()) {
        THROW(std::invalid_argument("empty video path"));
    }

    if (!m_renderer.openFromFile(m_video_path)) {
        std::cout << "Opened the file\n";
        THROW(std::runtime_error("can't open video file"));
    }

    // because xlib is NOT DOING #undef
    #undef None
    auto m_monitor_dimensions = m_monitor_info.getDimensions();

    auto width = static_cast<unsigned int>(m_monitor_dimensions.x) - 1;
    auto height = static_cast<unsigned int>(m_monitor_dimensions.y);

    auto m_monitor_positions = m_monitor_info.getPosition();

        auto m_monitor_positions = m_monitor_info.getPosition();

    if (m_renderwindow == nullptr) {
        xppr::log::info("Creating new window for drawing\n");

        m_renderwindow = new sf::RenderWindow(sf::VideoMode(width, height),
                                                "X-papers", sf::Style::None);
    } else {
        m_renderwindow->setSize({width, height});
    }

    m_renderwindow->setPosition({x_pos, y_pos});
}

void VideoHandler::start(
    std::function<void(VideoHandler* video, sf::Event& ev)> func) {
    auto manager = XDisplayHandler::getInstance();
    m_xlib_window = manager->addWindow(m_renderwindow->getSystemHandle());
    setCorrectWindowProperties();

    m_renderwindow->setFramerateLimit(m_fps_limit);
    m_renderwindow->setVerticalSyncEnabled(m_vert_sync_enabled);

    m_renderer.fit(0, 0, static_cast<float>(m_renderwindow->getSize().x),
                   static_cast<float>(m_renderwindow->getSize().y));
    m_renderer.play();

    while (m_renderwindow->isOpen()) {
        sf::Event ev;
        if (!(m_renderer.getStatus() == sfe::Playing)) {
            m_renderer.play();
        }

        while (m_renderwindow->pollEvent(ev)) {
            // handle events by user
            func(this, ev);
        }

        m_renderer.update();

        m_renderwindow->clear();
        m_renderwindow->draw(m_renderer);
        m_renderwindow->display();
    }

    manager->closeDisplay();
}

void VideoHandler::setFPSlimit(const uint32_t new_limit) {
    xppr::log::info("FPS limit set at %u", new_limit);

    m_fps_limit = new_limit;
}

void VideoHandler::enableVerticalSync() {
    xppr::log::info("Vertical Sync enabled");

    m_vert_sync_enabled = true;
}

bool VideoHandler::isOpen() const {
    return m_renderwindow->isOpen();
}

std::size_t VideoHandler::getFPSlimit() const {
    return m_fps_limit;
}

std::string VideoHandler::getVideoPath() const {
    return m_video_path;
}

bool VideoHandler::isVertSyncEnabled() const {
    return m_vert_sync_enabled;
}

sf::RenderWindow* VideoHandler::getRenderWindow() {
    return m_renderwindow;
}

XWindowHandler* VideoHandler::getHandler() {
    return m_xlib_window;
}

void VideoHandler::close() {
    m_renderwindow->close();
}

void VideoHandler::fit(float x,
                       float y,
                       float width,
                       float height,
                       bool preserve_ratio) {
    m_renderer.fit(x, y, width, height, preserve_ratio);
}

void VideoHandler::setView(const sf::View& view) {
    m_renderwindow->setView(view);
}

sf::Vector2u VideoHandler::getSize() const {
    return m_renderwindow->getSize();
}

sfe::Movie& VideoHandler::getRenderer() {
    return m_renderer;
}

int VideoHandler::setWindowTypeDesktop(
    std::shared_ptr<XDisplayHandler>& manager) {
    auto wintype = manager->createAtom("_NET_WM_WINDOW_TYPE", False);

    auto desktop = manager->createAtom("_NET_WM_WINDOW_TYPE_DESKTOP", False);

    return m_xlib_window->changeProperty(
        wintype, XA_ATOM, 32, PropModeReplace,
        reinterpret_cast<unsigned char*>(&desktop), 1);
}

int VideoHandler::setWindowOnAllDesktops(
    std::shared_ptr<XDisplayHandler>& manager) {
    unsigned int ints[2];

    ints[0] = 0xFFFFFFFF;
    ints[1] = 2;

    auto wm_desktop = manager->createAtom("_NET_WM_DESKTOP", False);

    return m_xlib_window->changeProperty(
        wm_desktop, XA_ATOM, 32, PropModeReplace,
        reinterpret_cast<unsigned char*>(ints), 2);
}

void VideoHandler::setCorrectWindowProperties() {
    auto manager = winengine::XDisplayHandler::getInstance();

    setWindowTypeDesktop(manager);

    setWindowOnAllDesktops(manager);
}

VideoHandler::~VideoHandler() {
    if (m_renderwindow != nullptr) {
        delete m_renderwindow;
    }
}

}  // namespace winengine
