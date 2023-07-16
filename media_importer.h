#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "media_base/raw_video_frame.h"

class MediaImporter {
public:
    bool open(int width, int height);
    media_base::RawVideoFrame *read_frame();
    void start_working();
private:
    void decode_compressed_frame();
private:
    mutable std::mutex _last_video_frame_mutex{};
    mutable media_base::RawVideoFrame *_last_video_frame{nullptr};
private:
    std::thread *_decoder_thread{nullptr};
    std::atomic<bool> _should_exit_decoder{false};
private:
    int _sock_fd;
    int _width;
    int _height;
};