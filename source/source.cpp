#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

const int kUdpPort = 10000;

int main() {
    FILE *fp = fopen("/home/tbago/media_resource/output.yuv", "rb");

    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;  // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(kUdpPort);

    socklen_t len = sizeof(servaddr);

    uint16_t index = 0;
    int width = 1280;
    int height = 720;

    int buffer_size = width + sizeof(int16_t);
    char buffer[buffer_size];

    int uv_frame_size = width * height / 2;
    while (true) {
        buffer[0] = (int8_t)(index % 128);
        buffer[1] = (int8_t)(index / 128);
        int size = fread(buffer + 2, 1, width, fp);
        if (size < width) {
            std::cout << "read end of file" << std::endl;
            break;
        }
        //std::cout << "buffer size " << (int32_t)(index >> 8) << std::endl;
        sendto(sockfd, (const char *)buffer, buffer_size, MSG_CONFIRM,
               (const struct sockaddr *)&servaddr, sizeof(servaddr));
        index++;

        if (index == height) {
            //read next frame
            index = 0;
            fseek(fp, uv_frame_size, SEEK_CUR);
            usleep(20000);
        }
    }

    fclose(fp);
    return 0;
}
