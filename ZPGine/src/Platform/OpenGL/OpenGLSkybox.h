//
// Created by phatt on 11/1/25.
//

#pragma once

#include "Skybox/Skybox.h"

namespace ZPG {

class OpenGLSkybox : public Skybox {
public:
    OpenGLSkybox(const SkyboxSpecification& spec);
    ~OpenGLSkybox() override;

    void Bind() override;
    void Unbind() override;

    const ref<VertexArray>& GetVertexArray() const override;
    void BindCubemapToSlot(int slot) const override;
    void UnbindCubemap() const override;

private:
    SkyboxSpecification m_Specification;
    u32 m_TextureRendererID = 0;

    ref<VertexArray> m_VAO = nullptr;
    ref<ShaderProgram> m_ShaderProgram = nullptr;
};

} // ZPG
