#pragma once

namespace ZPG {

struct AttachmentType {
    enum Type {
        Color,
        DepthStencil,
        Depth,
        Stencil,
    };

    struct GLAttachmentMapping {
        GLenum Attachment;
    };

    static GLAttachmentMapping ToGL(AttachmentType::Type attachmentType);
};

}
