#pragma once

namespace ZPG {

struct Attachment {
    enum AttachmentType {
        Color,
        DepthStencil,
        Depth,
        Stencil,
    };

    AttachmentType Type = Color;
    u32 Index = 0;

    struct GLAttachmentMapping {
        GLenum Attachment;
    };

    static GLAttachmentMapping ToGL(Attachment::AttachmentType attachmentType);

    bool operator==(const Attachment& other) const noexcept {
        return Type == other.Type && Index == other.Index;
    }
};


}


namespace std {

// hash specialization for my ZPG::Attachment

template<>
struct hash<ZPG::Attachment> {
    std::size_t operator()(const ZPG::Attachment& attachment) const noexcept {
        std::size_t h1 = std::hash<int>{}(attachment.Type);
        std::size_t h2 = std::hash<ZPG::u32>{}(attachment.Index);

        return h1 ^ (h2 << 1);
    }
};

}
