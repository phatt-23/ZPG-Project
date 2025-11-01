#pragma once

namespace ZPG {

enum class DataFormat : u32 {
    None = 0,
    R8,
    RG8,
    RGB8,
    RGB16F,
    RGB32F,
    RGBA8,
    RGBA16F,
    RGBA32F,
    Depth24Stencil8,
    Depth32F,
};

enum class AttachmentType : u32 {
    Color,
    Depth,
    Stencil,
    DepthStencil,
};

struct BufferSpecification {

    explicit BufferSpecification(DataFormat format);

    AttachmentType Type = AttachmentType::Color;
    DataFormat Format = DataFormat::RGBA8;

    bool operator==(const BufferSpecification& other) const;

private:
    static AttachmentType DeduceBufferType(DataFormat format);
};

}
