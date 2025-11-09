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

    FrameBufferAttachment::FrameBufferAttachment(TextureDataFormat format, u32 index)
        : AttachmentType(Utils::DeduceBufferType(format))
        , DataFormat(format)
        , Index(index)
    {
        ZPG_PROFILE_FUNCTION();

    }

    FrameBufferAttachment::FrameBufferAttachment(FrameBufferAttachmentType type, TextureDataFormat format, u32 index)
        : AttachmentType(type)
          , DataFormat(format)
          , Index(index)
    {
        ZPG_PROFILE_FUNCTION();

    }

    bool FrameBufferAttachment::operator==(const FrameBufferAttachment& other) const
    {
        ZPG_PROFILE_FUNCTION();
        return DataFormat == other.DataFormat && AttachmentType == other.AttachmentType && Index == other.Index;
    }

    std::string FrameBufferAttachment::operator()() const
    {
        ZPG_PROFILE_FUNCTION();
        switch (AttachmentType)
        {
            case FrameBufferAttachmentType::Color:         return "COLOR" + fmt::to_string(Index);
            case FrameBufferAttachmentType::DepthStencil:  return "DEPTH_STENCIL" + fmt::to_string(Index);
            case FrameBufferAttachmentType::Depth:         return "DEPTH" + fmt::to_string(Index);
            case FrameBufferAttachmentType::Stencil:       return "STENCIL" + fmt::to_string(Index);
        }
        ZPG_UNREACHABLE();
    }


}

std::size_t std::hash<ZPG::FrameBufferAttachment>::operator()(const ZPG::FrameBufferAttachment& attachment) const noexcept
{
    ZPG_PROFILE_FUNCTION();

    std::size_t h1 = std::hash<ZPG::u32>{}((ZPG::u32)attachment.AttachmentType);
    std::size_t h2 = std::hash<ZPG::u32>{}((ZPG::u32)attachment.DataFormat);
    std::size_t h3 = std::hash<ZPG::u32>{}(attachment.Index);

    return h1 | (h2 << 4) | (h3 << 8);
}
