#pragma once

#include "ZPGine.h"

class ObjectsLayer : public ZPG::Layer {
public:
    ObjectsLayer();
    void OnAttach() override;
    void OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) override;
    void OnRender(const ZPG::RenderContext& ctx) override;

private:
    ZPG::Ref<ZPG::ShaderProgram> m_ShaderProgram;
    ZPG::Ref<ZPG::VertexArray> m_VAO;

    ZPG::Ref<ZPG::CompoundTransform> m_Transform;
};
