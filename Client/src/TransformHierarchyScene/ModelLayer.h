#pragma once

#include "ZPGine.h"

namespace TransformHierarchyNS {
    
class ModelLayer : public ZPG::Layer {
public:
    ModelLayer();
    void OnAttach() override;
    void OnUpdate(ZPG::SceneContext& context) override;
    void OnRender(const ZPG::RenderContext& context) override;
private:
    ZPG::ref<ZPG::VertexArray> m_BoxVAO;
};

}
