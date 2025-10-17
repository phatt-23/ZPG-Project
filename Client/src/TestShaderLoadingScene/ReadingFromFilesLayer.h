#pragma once

#include "ZPGine.h"

class ReadingFromFilesLayer : public ZPG::Layer {
public:
    ReadingFromFilesLayer();
    void OnAttach() override;
    void OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) override;
    void OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) override;
private: 
    ZPG::ref<ZPG::ShaderProgram> m_ShaderProgram;
    ZPG::ref<ZPG::VertexArray> m_TreeVAO;
};
