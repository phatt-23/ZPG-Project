//
// Created by phatt on 10/29/25.
//

#include "BufferSpecification.h"

#include "Debug/Asserter.h"

namespace ZPG {

BufferSpecification::BufferSpecification(DataFormat format)
    : Type(DeduceBufferType(format))
    , Format(format)
{

}

bool BufferSpecification::operator==(const BufferSpecification& other) const {
    return Format == other.Format && Type == other.Type;
}

AttachmentType BufferSpecification::DeduceBufferType(DataFormat format) {
    switch (format) {
        case DataFormat::Depth32F:          return AttachmentType::Depth;
        case DataFormat::Depth24Stencil8:   return AttachmentType::DepthStencil;
        default:                            return AttachmentType::Color;
    }

    ZPG_UNREACHABLE();
}

}
