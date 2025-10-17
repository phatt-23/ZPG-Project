#pragma once

#include "ZPGine.h"

class InlineShadersLayer : public ZPG::Layer {
public:
    InlineShadersLayer();
    void OnAttach() override;
    void OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) override;
    void OnRender(const ZPG::RenderContext& ctx) override;
private: 
    ZPG::ref<ZPG::ShaderProgram> m_ShaderProgram;
    ZPG::ref<ZPG::VertexArray> m_TreeVAO;
};
