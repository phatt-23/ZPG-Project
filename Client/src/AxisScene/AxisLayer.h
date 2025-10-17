#pragma once

#include "ZPGine.h"

namespace AxisSceneNS {

class AxisLayer final : public ZPG::Layer {
public:
    void OnAttach() override;
    void OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) override;
private:
    ZPG::ref<ZPG::Entity> m_Entity;
    ZPG::ref<ZPG::ShaderProgram> m_ShaderProgram;
};

}
