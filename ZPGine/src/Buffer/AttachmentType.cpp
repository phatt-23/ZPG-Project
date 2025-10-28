#include "AttachmentType.h"

#include "Debug/Asserter.h"

namespace ZPG {

using AT = AttachmentType;

AT::GLAttachmentMapping AT::ToGL(AT::Type attachmentType) {
    switch (attachmentType) {
        case Color:         return { GL_COLOR_ATTACHMENT0 };
        case DepthStencil:  return { GL_DEPTH_STENCIL_ATTACHMENT };
        case Depth:         return { GL_DEPTH_ATTACHMENT };
        case Stencil:       return { GL_STENCIL_ATTACHMENT };
        default: ZPG_UNREACHABLE();
    }
}

}
