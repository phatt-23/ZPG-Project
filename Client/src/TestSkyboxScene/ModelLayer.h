#pragma once

#include "ZPGine.h"

namespace TestSkyboxNS {
    
class ModelLayer : public ZPG::Layer {
public:
    ModelLayer();
    void OnAttach() override;
    void OnUpdate(ZPG::SceneContext& context) override;
    void OnRender(const ZPG::RenderContext& context) override;
private:
    ZPG::Ref<ZPG::VertexArray> m_BoxVAO;
};

}
