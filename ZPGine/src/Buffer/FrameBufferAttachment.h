#pragma once

#include "Texture/TextureDataFormat.h"
#include "Texture/TextureType.h"

namespace ZPG {

    enum class FrameBufferAttachmentType {
        Color,
        Depth,
        Stencil,
        DepthStencil,
    };

    struct FrameBufferAttachment
    {
        FrameBufferAttachmentType AttachType = FrameBufferAttachmentType::Color;
        TextureType TexType = TextureType::Texture2D;
        TextureDataFormat DataFormat = TextureDataFormat::RGBA8;
        u32 Index = 0;
        u32 ArraySize = 1;

        bool operator==(const FrameBufferAttachment& other) const;
        std::string operator()() const;
    };

}

template <>
struct std::hash<ZPG::FrameBufferAttachment> {
    std::size_t operator()(const ZPG::FrameBufferAttachment& attachment) const noexcept;
};
