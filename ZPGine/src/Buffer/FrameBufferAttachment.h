#pragma once

#include "Texture/TextureDataFormat.h"

namespace ZPG {

enum class FrameBufferAttachmentType {
    Color,
    Depth,
    Stencil,
    DepthStencil,
};

struct FrameBufferAttachment {
    FrameBufferAttachment(TextureDataFormat format, u32 index = 0);
    FrameBufferAttachment(FrameBufferAttachmentType type, TextureDataFormat format, u32 index = 0);

    FrameBufferAttachmentType AttachmentType = FrameBufferAttachmentType::Color;
    TextureDataFormat DataFormat = TextureDataFormat::RGBA8;
    u32 Index = 0;

    bool operator==(const FrameBufferAttachment& other) const;
    std::string operator()() const;
};
}

template <>
struct std::hash<ZPG::FrameBufferAttachment> {
    std::size_t operator()(const ZPG::FrameBufferAttachment& attachment) const noexcept;
};
