#include "media_importer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>

#include "base/log.h"

static int kUdpPort = 10000;

bool MediaImporter::open(int width, int height) {
    if (_sock_fd = socket(AF_INET, SOCK_DGRAM, 0), _sock_fd < 0) {
        base::LogError() << "socket creation failed";
        return false;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;  // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(kUdpPort);

    // Bind the socket with the server address
    if (bind(_sock_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        base::LogError() << "bind failed";
        return false;
    }

    _last_video_frame =
        new media_base::RawVideoFrame(media_base::PixelFormat::PixelFormatYUV420P, width, height);

    FILE *fp = fopen("/home/tbago/media_resource/output.yuv", "rb");
    int y_frame_data_size = width * height;
    int8_t *y_frame_data = (int8_t *)malloc(y_frame_data_size);

    int uv_frame_size = width * height / 2;
    int8_t *uv_frame_data = (int8_t *)malloc(uv_frame_size);

    int seek_pos = (y_frame_data_size + uv_frame_size) * 30;
    fseek(fp, seek_pos, SEEK_SET);

    fread(y_frame_data, 1, y_frame_data_size, fp);

    _last_video_frame->PushFrameData(width, y_frame_data);

    //fread(uv_frame_data, 1, uv_frame_size / 2, fp);
    memset(uv_frame_data, 128, uv_frame_size);
    _last_video_frame->PushFrameData(width, uv_frame_data);
    _last_video_frame->PushFrameData(width, uv_frame_data);

    fclose(fp);
    _width = width;
    _height = height;
    return true;
}

media_base::RawVideoFrame *MediaImporter::read_frame() {
    std::lock_guard<std::mutex> lock(_last_video_frame_mutex);
    if (_last_video_frame == nullptr) {
        return nullptr;
    }
    media_base::RawVideoFrame *copy_frame = new media_base::RawVideoFrame(*_last_video_frame);
    return copy_frame;
}

void MediaImporter::start_working() {
    _should_exit_decoder = false;
    _decoder_thread = new std::thread(&MediaImporter::decode_compressed_frame, this);
}

const int kMaxDataSize = 1400;
void MediaImporter::decode_compressed_frame() {
    struct sockaddr_in serveraddr;
    unsigned int len = sizeof(serveraddr);  //len is value/result
    char buffer[kMaxDataSize];

    int frame_data_size = _width * _height;
    int8_t *y_frame_data = (int8_t *)malloc(frame_data_size);
    int need_package_count = 0;
    while (!_should_exit_decoder) {
        int size = recvfrom(_sock_fd, buffer, kMaxDataSize, MSG_WAITALL,
                            (struct sockaddr *)&serveraddr, &len);

        std::cout << "receive data size : " << size << std::endl;
        //first byte is index
        size -= 1;
        int8_t index = buffer[0];

        int y_frame_index = index * size;
        memcpy(y_frame_data + y_frame_index, buffer + 1, size - 1);
        //receive one frame complete
        if (y_frame_index == frame_data_size - size) {
            {
                std::lock_guard<std::mutex> lock(_last_video_frame_mutex);
                _last_video_frame->SetFrameData(0, y_frame_data, frame_data_size);
            }
        }
    }

    free(y_frame_data);
}