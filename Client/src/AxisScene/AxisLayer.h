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
    ZPG::Scope<ZPG::Model> m_Model;
    ZPG::Ref<ZPG::ShaderProgram> m_ShaderProgram;
};

}
