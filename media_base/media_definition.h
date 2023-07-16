#pragma once

#include <cstdint>

namespace media_base {

/**
 *  Stream type enumeration
 */
enum class StreamType {
    StreamTypeUnknown,   ///< unknown stream type
    StreamTypeVideo,     ///< video stream
    StreamTypeAudio,     ///< audio stream
    StreamTypeSubtitle,  ///< subtitle stream
};

/**
 * rational type
 */
struct Rational {
    int32_t numerator;
    int32_t denominator;
};

enum class CodecID {
    CodecIDNone,
    CodecIDH264,
    CodecIDHEVC,
    CodecIDAAC,
    CodecIDAC3,
};

enum class PixelFormat {
    PixelFormatNone,
    PixelFormatYUV420P,  ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    PixelFormatYUV422P,  ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    PixelFormatYUV444P,  ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
    PixelFormatNV12,  ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
    PixelFormatRGB24,  ///< packed RGB 8:8:8, 24bpp, RGBRGB
    PixelFormatBGR24,  ///< packed RGB 8:8:8, 24bpp, BGRBGR
};

enum class SampleFormat {
    SampleFormatNone,
    SampleFormatU8,   ///< unsigned 8 bits
    SampleFormatS16,  ///< signed 16 bits
    SampleFormatS32,  ///<signed 32 bits
    SampleFormatFLT,  ///< float
    SampleFormatDBL,  ///< double
    SampleFormatS64,  ///< signed 64 bits

    SampleFormatU8P,   ///< unsigned 8 bits, planar
    SampleFormatS16P,  ///< signed 16 bits, planar
    SampleFormatS32P,  ///< signed 32 bits, planar
    SampleFormatFLTP,  ///< float, planar
    SampleFormatDBLP,  ///< double, planar
    SampleFormatS64P,  ///< signed 64 bits, planar
};

}  // namespace media_base