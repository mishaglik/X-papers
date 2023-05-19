#ifndef X11_VIDEO_HANDLER_HPP
#define X11_VIDEO_HANDLER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <functional>
#include <sfeMovie/Movie.hpp>
#include "WinEngine/WindowEngine.hpp"
#include "WinEngine/XWindowHandler.hpp"

namespace winengine {

class VideoHandler {
   public:
    VideoHandler() = delete;

    explicit VideoHandler(const std::string& path, MonitorInfo& monitor);

    void start(std::function<void(VideoHandler* video, sf::Event& ev)> func);

    void setFPSlimit(const uint32_t new_limit);

    void enableVerticalSync();

    bool isOpen() const;

    std::size_t getFPSlimit() const;

    std::string getVideoPath() const;

    bool isVertSyncEnabled() const;

    // DEV-only functions (DO NOT USE IN YOUR PROGRAM)

    sf::RenderWindow* getRenderWindow();

    XWindowHandler* getHandler();

    // end of DEV-only functions

    void close() { m_renderwindow->close(); }

    void fit(float x,
             float y,
             float width,
             float height,
             bool preserve_ratio = true) {
        m_renderer.fit(x, y, width, height, preserve_ratio);
    }

    void setView(const sf::View& view) { m_renderwindow->setView(view); }

    sf::Vector2u getSize() const { return m_renderwindow->getSize(); }

    sfe::Movie& getRenderer() { return m_renderer; }

   private:
    std::string m_video_path;
    sfe::Movie m_renderer;

    MonitorInfo m_monitor_info;

    bool m_vert_sync_enabled = false;
    uint32_t m_fps_limit = 30;

    sf::RenderWindow* m_renderwindow;
    XWindowHandler* m_xlib_window;
};

}  // namespace winengine

#endif  // !X11_VIDEO_HANDLER_HPP
