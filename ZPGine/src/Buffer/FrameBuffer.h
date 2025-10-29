//
// Created by phatt on 10/18/25.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Attachment.h"

namespace ZPG {

class RenderBuffer;
class Texture;

class FrameBuffer {
    template<typename T>
    using AttachMap = std::unordered_map<Attachment, T>;

public:
    FrameBuffer(
        const AttachMap<ref<Texture>>& textureAttachments,
        const AttachMap<ref<RenderBuffer>>& rboAttachments);

    ~FrameBuffer();

    void Bind();

    /**
     * Binds the default FBO (0).
     */
    void Unbind();

    const AttachMap<ref<Texture>>& GetTextureAttachments() const;
    const AttachMap<ref<Texture>>& GetColorTextureAttachments() const;
    const AttachMap<ref<RenderBuffer>>& GetRenderBufferAttachments() const;

    /**
     * Binds the textures attached as Color attachments
     * to their corresponding texture index.
     */
    void BindColorTextureAttachments();

private:
    u32 m_RendererID;

    AttachMap<ref<Texture>> m_TextureAttachments;
    AttachMap<ref<RenderBuffer>> m_RBOAttachments;

    AttachMap<ref<Texture>> m_ColorTextureAttachments;
};

}


#endif //FRAMEBUFFER_H
