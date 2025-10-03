#pragma once

#include "ZPGine.h"

class TreeLayer : public ZPG::Layer {
public:
    TreeLayer();
    void OnAttach() override;
    void OnUpdate(ZPG::SceneContext& context) override;
    void OnRender(const ZPG::RenderContext& ctx) override;
private:
    ZPG::Ref<ZPG::VertexArray> m_TreeVAO;
    ZPG::Ref<ZPG::VertexArray> m_PlaneVao;
    ZPG::Ref<ZPG::ShaderProgram> m_NormalShaderProgram;
    ZPG::Ref<ZPG::ShaderProgram> m_RedShaderProgram;
};

