#pragma once

#include "ZPGine.h"

namespace TestModelSceneNS {

class ObjectsLayer : public ZPG::Layer {
public:
    ObjectsLayer();
    void OnAttach() override;
    void OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) override;
    void OnRender(const ZPG::RenderContext& ctx) override;

private:
    ZPG::ref<ZPG::ShaderProgram> m_ShaderProgram;
    ZPG::ref<ZPG::Model> m_Model;
    ZPG::ref<ZPG::Entity> m_Entity;
};

}