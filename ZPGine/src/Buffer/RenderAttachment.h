#pragma once
#include "BufferSpecification.h"

namespace ZPG {

    struct RenderAttachment {
        RenderAttachment();
        RenderAttachment(const BufferSpecification& bufferSpec, u32 bindingPoint);

        BufferSpecification BufferSpec;
        u32 BindingPoint = 0;

        std::string ToString() const;
        bool operator==(const RenderAttachment& other) const noexcept;
    };

}

template<>
struct std::hash<ZPG::RenderAttachment> {
    std::size_t operator()(const ZPG::RenderAttachment& attachment) const noexcept;
};

