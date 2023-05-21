#ifndef ENGINE_WINENGINE_ENGINE_HPP
#define ENGINE_WINENGINE_ENGINE_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "WinEngine/VideoHandler.hpp" // ORDER IS IMPORTANT, THIS MUST BE FIRST
#include "WinEngine/XWindowHandler.hpp"
#include "Utilities/log.hpp"
#include "WinEngine/WindowEngine.hpp"
#include "WinEngine/XDisplayHandler.hpp"
#include "glfwRender.hpp"


winengine::XWindowHandler* test_open_window();

winengine::XWindowHandler* openSimpleWindow(winengine::MonitorInfo*);

void test_video_handler(const char*);

winengine::XWindowHandler* test_open_window();

void test_gl(const char*);

#endif /* ENGINE_WINENGINE_ENGINE_HPP */
