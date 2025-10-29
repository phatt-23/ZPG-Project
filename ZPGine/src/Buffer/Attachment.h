#pragma once

namespace ZPG {

struct AttachmentType {
    enum Type {
        Color,
        DepthStencil,
        Depth,
        Stencil,
    };

    Type m_Type;
    u32 m_Index;

    struct GLAttachmentMapping {
        GLenum Attachment;
    };

    static GLAttachmentMapping ToGL(AttachmentType::Type attachmentType);
};

}
