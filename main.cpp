#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#include "base/log.h"
#include "media_importer.h"

void usage(const char *bin_name);
bool is_integer(const std::string &tested_integer);

int main(int argc, const char *argv[]) {
    int window_width = 1280;
    int window_height = 720;
    for (int i = 1; i < argc; i++) {
        const std::string current_arg = argv[i];
        if (current_arg == "--help") {
            usage(argv[0]);
            return 0;
        } else if (current_arg == "-w" || current_arg == "--width") {
            if (argc <= i + 1) {
                usage(argv[0]);
                return 1;
            }

            const std::string width_string(argv[i + 1]);
            i++;
            if (!is_integer(width_string)) {
                usage(argv[0]);
                return 1;
            }
            window_width = std::stoi(width_string);
        } else if (current_arg == "-h" || current_arg == "--height") {
            if (argc <= i + 1) {
                usage(argv[0]);
                return 1;
            }
            const std::string height_string(argv[i + 1]);
            i++;
            if (!is_integer(height_string)) {
                usage(argv[0]);
                return 1;
            }
            window_height = std::stoi(height_string);
        } else {
            std::cout << "Invalid arg " << current_arg << std::endl;
            usage(argv[0]);
            return 1;
        }
    }

    MediaImporter importer;
    if (!importer.open(window_width, window_height)) {
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        base::LogError() << "SDL init failed";
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("MAVPlayer", 0, SDL_WINDOWPOS_CENTERED, window_width,
                                          window_height, SDL_WINDOW_SHOWN);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = window_width;
    rect.h = window_height;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        base::LogError() << "Cannot create render";
        return 1;
    }
    Uint32 pixel_format = SDL_PIXELFORMAT_IYUV;
    SDL_Texture *texture =
        SDL_CreateTexture(renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
    if (texture == NULL) {
        base::LogError() << "Create texture failed";
        return 1;
    }

    bool isquit = false;
    SDL_Event event;

    importer.start_working();

    while (!isquit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isquit = true;
            }
        }

        media_base::RawVideoFrame *video_frame = importer.read_frame();
        if (video_frame != nullptr) {
            SDL_UpdateYUVTexture(texture, NULL,
                                 (const Uint8 *)video_frame->frame_buffers[0]->frame_data,
                                 video_frame->frame_buffers[0]->line_size,
                                 (const Uint8 *)video_frame->frame_buffers[1]->frame_data,
                                 video_frame->frame_buffers[1]->line_size,
                                 (const Uint8 *)video_frame->frame_buffers[2]->frame_data,
                                 video_frame->frame_buffers[2]->line_size);
            delete video_frame;

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(5);
        } else {
            SDL_Delay(1);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void usage(const char *bin_name) {
    std::cout << "Usage: " << bin_name << '\n'
              << '\n'
              << "Options:" << '\n'
              << "  --help          : show this help" << '\n'
              << "  -v | --version  : show version information " << '\n'
              << "  -f | --file     : set the play file path" << '\n'
              << "  -w | --width    : set window width same as video resolution width" << '\n'
              << "  -h | --height   : set window height same as video resolution height" << '\n';
}

bool is_integer(const std::string &tested_integer) {
    for (const auto &digit : tested_integer) {
        if (!std::isdigit(digit)) {
            return false;
        }
    }

    return true;
}
