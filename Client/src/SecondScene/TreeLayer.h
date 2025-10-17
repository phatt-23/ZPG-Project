#pragma once

#include "ZPGine.h"

class TreeLayer : public ZPG::Layer {
public:
    TreeLayer();
    void OnAttach() override;
    void OnUpdate(ZPG::SceneContext& context) override;
    void OnRender(const ZPG::RenderContext& ctx) override;
private:
    ZPG::ref<ZPG::VertexArray> m_TreeVAO;
    ZPG::ref<ZPG::VertexArray> m_PlaneVao;
    ZPG::ref<ZPG::ShaderProgram> m_NormalShaderProgram;
    ZPG::ref<ZPG::ShaderProgram> m_RedShaderProgram;
};

