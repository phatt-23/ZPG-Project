#pragma once

namespace ZPG {

struct BufferDataFormat {

    enum BufferFormat {
        None = 0,
        R8,
        RG8,
        RGB8,
        RGB16F,
        RGB32F,
        RGBA8,
        RGBA16F,
        RGBA32F,
        DEPTH24_STENCIL8,
        DEPTH32F,
    };

    enum BufferType {
        Color,
        Depth,
        Stencil,
        DepthStencil,
    };

    BufferFormat Format;
    BufferType Type;

    BufferDataFormat(BufferFormat format = BufferFormat::RGBA8);

    struct GLFormatMapping {
        GLenum InternalFormat;
        GLenum Format;
        GLenum SampleType;
        GLenum SampleTypeSize;
    };

    static GLFormatMapping ToGL(BufferDataFormat::BufferFormat format);

private:
    static BufferType DeduceBufferType(BufferFormat format);
};

}
