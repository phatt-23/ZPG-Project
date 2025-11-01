#include "RenderAttachment.h"

#include "BufferSpecification.h"
#include "Debug/Asserter.h"

namespace ZPG {
RenderAttachment::RenderAttachment()
    : BufferSpec(DataFormat::None)
    , BindingPoint(0)
{
}

RenderAttachment::RenderAttachment(const BufferSpecification& bufferSpec, u32 bindingPoint)
    : BufferSpec(bufferSpec)
    , BindingPoint(bindingPoint)
{

}

std::string RenderAttachment::ToString() const {
        switch (BufferSpec.Type) {
            case AttachmentType::Color:         return "COLOR" + fmt::to_string(BindingPoint);
            case AttachmentType::DepthStencil:  return "DEPTH_STENCIL" + fmt::to_string(BindingPoint);
            case AttachmentType::Depth:         return "DEPTH" + fmt::to_string(BindingPoint);
            case AttachmentType::Stencil:       return "STENCIL" + fmt::to_string(BindingPoint);
            default: ZPG_UNREACHABLE();
        }
    }

    bool RenderAttachment::operator==(const RenderAttachment& other) const noexcept {
        return  BufferSpec.Format == other.BufferSpec.Format &&
            BufferSpec.Type == other.BufferSpec.Type &&
            BindingPoint == other.BindingPoint;
    }

}

std::size_t std::hash<ZPG::RenderAttachment>::operator()(const ZPG::RenderAttachment& attachment) const noexcept {
    std::size_t h1 = std::hash<ZPG::u32>{}((ZPG::u32)attachment.BufferSpec.Type);
    std::size_t h2 = std::hash<ZPG::u32>{}((ZPG::u32)attachment.BufferSpec.Format);
    std::size_t h3 = std::hash<ZPG::u32>{}(attachment.BindingPoint);

    return h1 | (h2 << 4) | (h3 << 8);
}


