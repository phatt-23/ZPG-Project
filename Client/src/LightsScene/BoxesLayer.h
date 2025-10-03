#pragma once

#include "ZPGine.h"

class BoxesLayer : public ZPG::Layer {
public:
    BoxesLayer();
    void OnAttach() override;
    void OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) override;
    void OnRender(const ZPG::RenderContext& ctx) override;

private:
    ZPG::Ref<ZPG::ShaderProgram> m_ShaderProgram;
    ZPG::Ref<ZPG::VertexArray> m_VAO;
};
