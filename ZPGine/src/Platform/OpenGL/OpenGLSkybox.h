//
// Created by phatt on 11/1/25.
//

#pragma once

#include "Sky/Skybox.h"

namespace ZPG {

class OpenGLSkybox : public Skybox {
public:
    OpenGLSkybox(const SkyboxSpecification& spec);
    ~OpenGLSkybox() override;

    void Bind() const override;
    void Unbind() const override;

    const ref<VertexArray>& GetVertexArray() const override;
    void BindTextureToSlot(int slot) const override;

private:

    SkyboxSpecification m_Specification;
    u32 m_TextureRendererID = 0;

    ref<VertexArray> m_VAO = nullptr;
    ref<ShaderProgram> m_ShaderProgram = nullptr;
};

} // ZPG
