#include "glfwVideo.hpp"
#include <X11/Xlib.h>
#include <algorithm>
#include "Utilities/log.hpp"

namespace winengine {

AVPixelFormat GLFWreaderState::correct_for_deprecated_pixel_format(
    AVPixelFormat pix_fmt) {
    // Fix swscaler deprecated pixel format warning
    // (YUVJ has been deprecated, change pixel format to regular YUV)
    switch (pix_fmt) {
        case AV_PIX_FMT_YUVJ420P:
            return AV_PIX_FMT_YUV420P;
        case AV_PIX_FMT_YUVJ422P:
            return AV_PIX_FMT_YUV422P;
        case AV_PIX_FMT_YUVJ444P:
            return AV_PIX_FMT_YUV444P;
        case AV_PIX_FMT_YUVJ440P:
            return AV_PIX_FMT_YUV440P;
        default:
            return pix_fmt;
    }
}

const char* GLFWreaderState::av_make_error(int32_t errnum) {
    static char str[AV_ERROR_MAX_STRING_SIZE];
    
    std::fill(str, str + sizeof(str), 0);

    return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}

GLFWreaderState::GLFWreaderState() {}

bool GLFWreaderState::Open(const std::string& filename) {
    this->m_filename = filename;

    auto& cur_width = this->m_width;

    auto& cur_height = this->m_height;

    auto& cur_time_base = this->m_av_rational_time_base;

    auto& cur_format_ctx = this->m_av_format_context;

    auto& cur_av_codec_ctx = this->m_av_codec_context;

    auto& cur_video_stream_index = this->m_stream_in_video_index;

    auto& cur_av_frame = this->m_av_frame;

    auto& cur_av_packet = this->m_av_packet;

    // Open the file using libavformat
    cur_format_ctx = avformat_alloc_context();
    if (!cur_format_ctx) {
        xppr::log::error("Couldn't create AVFormatContext\n");
        return false;
    }

    if (avformat_open_input(&cur_format_ctx, filename.c_str(), nullptr,
                            nullptr) != 0) {
        xppr::log::error("Couldn't open video file\n");
        return false;
    }

    // Find the first valid video stream inside the file
    cur_video_stream_index = -1;

    AVCodecParameters* av_codec_params;
    AVCodec* av_codec;

    for (std::size_t i = 0; i < cur_format_ctx->nb_streams; ++i) {
        av_codec_params = cur_format_ctx->streams[i]->codecpar;
        av_codec = const_cast<AVCodec*>(
            avcodec_find_decoder(av_codec_params->codec_id));

        if (!av_codec) {
            continue;
        }

        if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
            cur_video_stream_index = static_cast<int32_t>(i);

            cur_width = av_codec_params->width;
            cur_height = av_codec_params->height;

            cur_time_base = cur_format_ctx->streams[i]->time_base;
            break;
        }
    }

    if (cur_video_stream_index == -1) {
        xppr::log::error("Couldn't find valid video stream inside file\n");

        return false;
    }

    // Set up a codec context for the decoder
    cur_av_codec_ctx = avcodec_alloc_context3(av_codec);

    if (!cur_av_codec_ctx) {
        xppr::log::error("Couldn't create AVCodecContext\n");

        return false;
    }

    if (avcodec_parameters_to_context(cur_av_codec_ctx, av_codec_params) < 0) {
        xppr::log::error("Couldn't initialize AVCodecContext\n");

        return false;
    }

    if (avcodec_open2(cur_av_codec_ctx, av_codec, nullptr) < 0) {
        xppr::log::error("Couldn't open codec\n");

        return false;
    }

    cur_av_frame = av_frame_alloc();

    if (!cur_av_frame) {
        xppr::log::error("Couldn't allocate AVFrame\n");

        return false;
    }

    cur_av_packet = av_packet_alloc();

    if (!cur_av_packet) {
        xppr::log::error("Couldn't allocate AVPacket\n");

        return false;
    }

    return true;
}

bool GLFWreaderState::readFrame(uint8_t* frame_buffer, int64_t* pts) {
    auto& cur_width = this->m_width;

    auto& cur_height = this->m_height;

    auto& cur_format_ctx = this->m_av_format_context;

    auto& cur_av_codec_ctx = this->m_av_codec_context;

    auto& cur_video_stream_index = this->m_stream_in_video_index;

    auto& cur_av_frame = this->m_av_frame;

    auto& cur_av_packet = this->m_av_packet;

    int32_t response = 0;

    while (av_read_frame(cur_format_ctx, cur_av_packet) >= 0) {
        if (cur_av_packet->stream_index != cur_video_stream_index) {
            av_packet_unref(cur_av_packet);

            continue;
        }

        response = avcodec_send_packet(cur_av_codec_ctx, cur_av_packet);
        if (response < 0) {
            xppr::log::error("Failed to decode packet: %s\n",
                             av_make_error(response));

            return false;
        }

        response = avcodec_receive_frame(cur_av_codec_ctx, cur_av_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            av_packet_unref(cur_av_packet);

            continue;
        } else if (response < 0) {
            xppr::log::error("Failed to decode packet: %s\n",
                             av_make_error(response));

            return false;
        }

        av_packet_unref(cur_av_packet);
        break;
    }

    *pts = cur_av_frame->pts;

    // Set up sws scaler
    auto source_pix_fmt =
        correct_for_deprecated_pixel_format(cur_av_codec_ctx->pix_fmt);

    m_sws_scaler_ctx = sws_getContext(cur_width, cur_height, source_pix_fmt,
                                      cur_width, cur_height, AV_PIX_FMT_RGB0,
                                      SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!m_sws_scaler_ctx) {
        xppr::log::error("Couldn't initialize sw scaler\n");

        return false;
    }

    uint8_t* dest[4] = {frame_buffer, nullptr, nullptr, nullptr};

    int32_t dest_linesize[4] = {cur_width * 4, 0, 0, 0};

    sws_scale(m_sws_scaler_ctx, cur_av_frame->data, cur_av_frame->linesize, 0,
              cur_av_frame->height, dest, dest_linesize);

    return true;
}

bool GLFWreaderState::seekFrame(int64_t ts) {
    auto& av_format_ctx = this->m_av_format_context;

    auto& av_codec_ctx = this->m_av_codec_context;

    auto& video_stream_index = this->m_stream_in_video_index;

    auto& av_packet = this->m_av_packet;

    auto& av_frame = this->m_av_frame;

    av_seek_frame(av_format_ctx, video_stream_index, ts, AVSEEK_FLAG_BACKWARD);

    // av_seek_frame takes effect after one frame, so I'm decoding one here
    // so that the next call to video_reader_read_frame() will give the correct
    // frame
    int32_t response = 0;

    while (av_read_frame(av_format_ctx, av_packet) >= 0) {
        if (av_packet->stream_index != video_stream_index) {
            av_packet_unref(av_packet);

            continue;
        }

        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if (response < 0) {
            xppr::log::error("Failed to decode packet: %s\n",
                             av_make_error(response));

            return false;
        }

        response = avcodec_receive_frame(av_codec_ctx, av_frame);

        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            av_packet_unref(av_packet);

            continue;
        } else if (response < 0) {
            xppr::log::error("Failed to decode packet: %s\n",
                             av_make_error(response));

            return false;
        }

        av_packet_unref(av_packet);

        break;
    }

    return true;
}

void GLFWreaderState::Close() {
    sws_freeContext(m_sws_scaler_ctx);

    avformat_close_input(&m_av_format_context);

    avformat_free_context(m_av_format_context);

    av_frame_free(&m_av_frame);

    av_packet_free(&m_av_packet);

    avcodec_free_context(&m_av_codec_context);
}

}  // namespace winengine
