#include "media_base/raw_video_frame.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

namespace media_base {

RawVideoFrame::RawVideoFrame(PixelFormat pixel_format, int32_t width, int32_t height)
    : pixel_format(pixel_format), width(width), height(height) {
    time_stamp = -1;
    duration = -1;
}

RawVideoFrame::RawVideoFrame(const RawVideoFrame &videoFrame) {
    CopyInfo(videoFrame);
}

RawVideoFrame &RawVideoFrame::operator=(const RawVideoFrame &videoFrame) {
    if (this == &videoFrame) {
        return *this;
    }
    CopyInfo(videoFrame);
    return *this;
}

RawVideoFrame::~RawVideoFrame() {
    FreeFrameBuffer();
}

void RawVideoFrame::PushFrameData(int32_t line_size, int8_t *frame_data) {
    RawVideoFrameBuffer *frame_buffer = new RawVideoFrameBuffer();
    frame_buffer->line_size = line_size;
    uint32_t lineIndex = frame_buffers.size();
    switch (pixel_format) {
        case PixelFormat::PixelFormatYUV420P:
            if (lineIndex == 0) {
                frame_buffer->line_count = height;
            } else {
                frame_buffer->line_count = height >> 1;
            }
            break;
        case PixelFormat::PixelFormatRGB24:
        case PixelFormat::PixelFormatBGR24:
            frame_buffer->line_count = height;
            break;
        default:
            assert(false);
            break;
    }

    int32_t frame_data_length = frame_buffer->line_size * frame_buffer->line_count;
    frame_buffer->frame_data = (int8_t *)malloc(frame_data_length);
    memcpy(frame_buffer->frame_data, frame_data, frame_data_length);
    frame_buffers.push_back(frame_buffer);
}

bool RawVideoFrame::SetFrameData(int32_t index, int8_t *frame_data, int32_t frame_data_length) {
    if (index >= frame_buffers.size()) {
        return false;
    }
    RawVideoFrameBuffer *frame_buffer = frame_buffers[index];
    memcpy(frame_buffer->frame_data, frame_data, frame_data_length);
    return true;
}

void RawVideoFrame::CopyInfo(const RawVideoFrame &video_frame) {
    pixel_format = video_frame.pixel_format;
    width = video_frame.width;
    height = video_frame.height;
    time_stamp = video_frame.time_stamp;
    duration = video_frame.duration;

    FreeFrameBuffer();
    for (auto frame_buffer : video_frame.frame_buffers) {
        PushFrameData(frame_buffer->line_size, frame_buffer->frame_data);
    }
}

void RawVideoFrame::FreeFrameBuffer() {
    for (auto frame_buffer : frame_buffers) {
        free(frame_buffer->frame_data);
        delete frame_buffer;
    }
    frame_buffers.clear();
}

}  // namespace media_base
