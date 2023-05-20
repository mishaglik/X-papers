#include "glfwRender.hpp"
#include <stdexcept>
#include "Utilities/defines.hpp"
#include "Utilities/log.hpp"

namespace winengine {

GlfwController::GlfwController() : vr_state(), window() {
    if (!glfwInit()) {
        xppr::log::error("Couldn't init GLFW\n");

        THROW(std::runtime_error("GLFW init error"));
    }
}

void GlfwController::Start(const std::string& filename) {
    openFromFile(filename);

    createGLxWindow();

    generateTexture();

    allocateFrameBuffer();

    handleEvents();
}

bool GlfwController::openFromFile(const std::string& filename) {
    if (!vr_state.Open(filename)) {
        xppr::log::error(
            "Couldn't open video file (make sure you set a video file that "
            "exists)\n");

        THROW(std::runtime_error("Can't open from file"));

        return false;
    }

    return true;
}

bool GlfwController::createGLxWindow() {
    window = glfwCreateWindow(vr_state.m_width, vr_state.m_height, "X-Papers",
                              nullptr, nullptr);

    if (!window) {
        xppr::log::error("Couldn't open window\n");

        THROW(std::runtime_error("Can't create GLX window"));

        return false;
    }

    return true;
}

// TODO: make this func readable
void GlfwController::generateTexture() {
    glfwMakeContextCurrent(window);

    glGenTextures(1, &tex_handle);

    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

bool GlfwController::allocateFrameBuffer() {
    // Allocate frame buffer
    constexpr int ALIGNMENT = 128;

    frame_width = vr_state.m_width;
    frame_height = vr_state.m_height;

    if (posix_memalign(reinterpret_cast<void**>(&frame_data), ALIGNMENT,
                       frame_width * frame_height * 4) != 0) {
        xppr::log::error("Couldn't allocate frame buffer\n");

        THROW(std::runtime_error("Frame buffer bad alloc"));

        return false;
    }

    return true;
}

void GlfwController::handleEvents() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up orphographic projection
        int window_width = 0, window_height = 0;
        glfwGetWindowSize(window, &window_width, &window_height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0, window_width, window_height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        // Read a new frame and load it into texture
        int64_t pts;
        if (!vr_state.readFrame(frame_data, &pts)) {
            xppr::log::error("Couldn't load video frame\n");

            THROW(std::runtime_error("Error loading video frame"));
        }

        static bool first_frame = true;

        if (first_frame) {
            glfwSetTime(0.0);

            first_frame = false;
        }

        double pt_in_seconds =
            pts * static_cast<double>(vr_state.m_av_rational_time_base.num) /
            static_cast<double>(vr_state.m_av_rational_time_base.den);

        while (pt_in_seconds > glfwGetTime()) {
            glfwWaitEventsTimeout(pt_in_seconds - glfwGetTime());
        }

        glBindTexture(GL_TEXTURE_2D, tex_handle);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, frame_data);

        // Render whatever you want
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_handle);

        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);

        glVertex2i(0, 0);
        glTexCoord2d(1, 0);

        glVertex2i(frame_width, 0);
        glTexCoord2d(1, 1);

        glVertex2i(frame_width, frame_height);
        glTexCoord2d(0, 1);

        glVertex2i(0, frame_height);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vr_state.Close();
}

}  // namespace winengine
