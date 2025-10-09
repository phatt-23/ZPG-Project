#pragma once

#include "Models/box.h"
#include "ZPGine.h"

namespace AxisSceneNS {

class AxisLayer : public ZPG::Layer {
public:
    AxisLayer();
    ~AxisLayer() override;
    void OnAttach() override;
    void OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) override;
private:
    ZPG::Ref<ZPG::Entity> m_Entity;
    ZPG::Ref<ZPG::ShaderProgram> m_ShaderProgram;
};

}
