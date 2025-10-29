#include "Attachment.h"

#include "Debug/Asserter.h"

namespace ZPG {

using AT = Attachment;

AT::GLAttachmentMapping AT::ToGL(AT::AttachmentType attachmentType) {
    switch (attachmentType) {
        case Color:         return { GL_COLOR_ATTACHMENT0 };
        case DepthStencil:  return { GL_DEPTH_STENCIL_ATTACHMENT };
        case Depth:         return { GL_DEPTH_ATTACHMENT };
        case Stencil:       return { GL_STENCIL_ATTACHMENT };
        default: ZPG_UNREACHABLE();
    }
}

}
