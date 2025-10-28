//
// Created by phatt on 10/18/25.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "AttachmentType.h"

namespace ZPG {

class RenderBuffer;
class Texture;

class FrameBuffer {
public:
    FrameBuffer(
        std::unordered_multimap<AttachmentType::Type, ref<Texture>> textureAttachments,
        std::unordered_multimap<AttachmentType::Type, ref<RenderBuffer>> rboAttachments);

    ~FrameBuffer();

    void Bind();
    void Unbind();

public:
    u32 m_RendererID;

    std::unordered_multimap<AttachmentType::Type, ref<Texture>> m_TextureAttachments;
    std::vector<ref<Texture>> m_ColorTextureAttachments;

    std::unordered_multimap<AttachmentType::Type, ref<RenderBuffer>> m_RBOAttachments;
};

}


#endif //FRAMEBUFFER_H
