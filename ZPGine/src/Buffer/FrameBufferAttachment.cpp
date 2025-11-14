//
// Created by phatt on 10/29/25.
//

#include "FrameBufferAttachment.h"

#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

    namespace Utils
    {

        FrameBufferAttachmentType DeduceBufferType(TextureDataFormat format)
        {
            ZPG_PROFILE_FUNCTION();
            switch (format)
            {
                case TextureDataFormat::Depth32F:          return FrameBufferAttachmentType::Depth;
                case TextureDataFormat::Depth24Stencil8:   return FrameBufferAttachmentType::DepthStencil;
                default:                                   return FrameBufferAttachmentType::Color;
            }

            ZPG_UNREACHABLE();
        }

    }
#if 0
    FrameBufferAttachment::FrameBufferAttachment(
        FrameBufferAttachmentType attachmentType,
        TextureType textureType,
        TextureDataFormat dataFormat,
        u32 index
    )
        : AttachmentType(attachmentType)
        , Type(textureType)
        , DataFormat(dataFormat)
        , Index(index)
    {
        ZPG_PROFILE_FUNCTION();
    }

    FrameBufferAttachment::FrameBufferAttachment(TextureType textureType, TextureDataFormat dataFormat, u32 index)
        : AttachmentType(Utils::DeduceBufferType(dataFormat))
        ,  Type(textureType)
        , DataFormat(dataFormat)
        , Index(index)
    {

    }

    FrameBufferAttachment::FrameBufferAttachment(TextureDataFormat dataFormat, u32 index)
        : AttachmentType(Utils::DeduceBufferType(dataFormat))
        , DataFormat(dataFormat)
        , Index(index)
    {
    }
#endif

    bool FrameBufferAttachment::operator==(const FrameBufferAttachment& other) const
    {
        ZPG_PROFILE_FUNCTION();
        return DataFormat == other.DataFormat && AttachType == other.AttachType && Index == other.Index;
    }

    std::string FrameBufferAttachment::operator()() const
    {
        ZPG_PROFILE_FUNCTION();
        switch (AttachType)
        {
            case FrameBufferAttachmentType::Color:         return "COLOR" + fmt::to_string(Index);
            case FrameBufferAttachmentType::DepthStencil:  return "DEPTH_STENCIL" + fmt::to_string(Index);
            case FrameBufferAttachmentType::Depth:         return "DEPTH" + fmt::to_string(Index);
            case FrameBufferAttachmentType::Stencil:       return "STENCIL" + fmt::to_string(Index);
        }
        ZPG_UNREACHABLE();
        return "";
    }


}

std::size_t std::hash<ZPG::FrameBufferAttachment>::operator()(const ZPG::FrameBufferAttachment& attachment) const noexcept
{
    ZPG_PROFILE_FUNCTION();

    std::size_t h1 = std::hash<ZPG::u32>{}((ZPG::u32)attachment.AttachType);
    std::size_t h2 = std::hash<ZPG::u32>{}((ZPG::u32)attachment.DataFormat);
    std::size_t h3 = std::hash<ZPG::u32>{}(attachment.Index);

    return h1 | (h2 << 4) | (h3 << 8);
}
