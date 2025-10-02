#pragma once

#include "ZPGine.h"

class InlineShadersLayer : public ZPG::Layer {
public:
    InlineShadersLayer(ZPG::CameraController& controller);
    void OnAttach() override;
    void OnUpdate([[maybe_unused]] ZPG::Timestep ts) override;
private: 
    ZPG::CameraController& m_CameraController;
    ZPG::Ref<ZPG::ShaderProgram> m_ShaderProgram;
    ZPG::Ref<ZPG::VertexArray> m_TreeVAO;
};
