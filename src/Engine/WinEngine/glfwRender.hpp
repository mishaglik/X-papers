#ifndef X11_GLFW_RENDER_HPP
#define X11_GLFW_RENDER_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <memory>
#include <string>
#include <GLFW/glfw3.h>
#include "glfwVideo.hpp"

namespace winengine {

class GlfwController {
    public:
    GlfwController();

    bool Render(const std::string& filename);

    private:

    bool openFromFile(const std::string& filename);

    bool createGLxWindow();

    bool generateTexture();

    bool allocateFrameBuffer();

    void handleEvents();

    private:
    winengine::GLFWreaderState vr_state; 

    GLFWwindow* window = nullptr;

    uint8_t* frame_data = nullptr;
    
    GLuint tex_handle;

    int frame_width;

    int frame_height;

};

}

#endif // !X11_GLFW_RENDER_HPP
