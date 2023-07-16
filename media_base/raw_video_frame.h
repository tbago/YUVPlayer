#pragma once

#include <vector>

#include "media_base/media_definition.h"

namespace media_base {

struct RawVideoFrameBuffer {
    int8_t *frame_data;  // frame data with length lineSize * lineCount
    int32_t line_size;   // line byte count
    int32_t line_count;  // number of lines
};

/**
 *  decoded raw video frame
 */
struct RawVideoFrame {
    PixelFormat pixel_format;
    int32_t width;
    int32_t height;
    int64_t time_stamp;  // in microsecond
    int32_t duration;

    std::vector<RawVideoFrameBuffer *> frame_buffers;
public:
    /**
    *  init empty object and use PushFrameData to init data
    *
    *  @param pixel_format video pixel format
    *  @param width       video width
    *  @param height      video height
    *
    *  @return new raw video frame instance
    */
    RawVideoFrame(PixelFormat pixel_format, int32_t width, int32_t height);
    RawVideoFrame(const RawVideoFrame &video_frame);
    RawVideoFrame &operator=(const RawVideoFrame &video_frame);
    virtual ~RawVideoFrame();
public:
    void PushFrameData(int32_t line_size, int8_t *frame_data);
    //change frame data by index
    bool SetFrameData(int32_t index, int8_t *frame_data, int32_t frame_data_length);
private:
    void CopyInfo(const RawVideoFrame &video_frame);
    void FreeFrameBuffer();
};

}  // namespace media_base