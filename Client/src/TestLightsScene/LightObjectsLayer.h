#pragma once

#include "ZPGine.h"

class LightObjectsLayer : public ZPG::Layer {
public:
    LightObjectsLayer() {}
    void OnAttach() override;
    void OnRender(const ZPG::RenderContext& ctx) override;

private:
    ZPG::Ref<ZPG::VertexArray> m_VAO;
};

