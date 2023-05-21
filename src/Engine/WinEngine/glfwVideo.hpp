#ifndef X11_GLFW_VIDEO_HPP
#define X11_GLFW_VIDEO_HPP

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/System/String.hpp>
#include <cstdint>
#include <string>

extern "C" {

#include <inttypes.h>

#include <libavcodec/avcodec.h>

#include <libavformat/avformat.h>

#include <libswscale/swscale.h>

}

namespace winengine {

class GLFWreaderState {
    friend class GlfwController;

   public:
    explicit GLFWreaderState();

    bool readFrame(uint8_t* frame_buffer, int64_t* pts);

    bool seekFrame(int64_t ts);

    bool Open(const std::string& filename);

    void Close();

    static AVPixelFormat correct_for_deprecated_pixel_format(AVPixelFormat pix_fmt);

    // av_err2str returns a temporary array. This doesn't work in gcc.
    // This function can be used as a replacement for av_err2str.
    static const char* av_make_error(int errnum);

   public:
    int32_t m_width = 0;
    int32_t m_height = 0;

    AVRational m_av_rational_time_base;

   private:
    AVFormatContext* m_av_format_context;

    AVCodecContext* m_av_codec_context;

    int m_stream_in_video_index;

    AVFrame* m_av_frame;

    AVPacket* m_av_packet;

    SwsContext* m_sws_scaler_ctx;

    std::string m_filename;
};

}  // namespace winengine

#endif  // !X11_GLFW_VIDEO_HPP
