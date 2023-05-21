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
#include <cstdint>
#include <functional>
#include <sfeMovie/Movie.hpp>

#include "WinEngine/proto.hpp"
#include "WindowEngine.hpp"
#include "XWindowHandler.hpp"
#include "XDisplayHandler.hpp"

namespace winengine {

class VideoHandler {
   public:
    VideoHandler() = delete;

    ~VideoHandler();

    VideoHandler(const std::string& path, const win_t win_id);

    
    VideoHandler(const std::string& path, MonitorInfo& monitor);


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


    void close();


    void fit(float x,
             float y,
             float width,
             float height,
             bool preserve_ratio = true);


    void setView(const sf::View& view);


    sf::Vector2u getSize() const;


    sfe::Movie& getRenderer();


    private:

    void setCorrectWindowProperties();


    int setWindowTypeDesktop(std::shared_ptr<XDisplayHandler>& manager);


    int setWindowOnAllDesktops(std::shared_ptr<XDisplayHandler>& manager);
    

   private:
    std::string m_video_path;
    
    sfe::Movie m_renderer;

    MonitorInfo m_monitor_info;

    bool m_vert_sync_enabled = false;
    
    uint32_t m_fps_limit = 30;

    sf::RenderWindow* m_renderwindow;

    XWindowHandler* m_xlib_window;

    win_t m_win_id = 0;
};

}  // namespace winengine

#endif  // !X11_VIDEO_HANDLER_HPP
