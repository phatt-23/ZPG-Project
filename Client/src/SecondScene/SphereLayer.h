#pragma once

#include "ZPGine.h"

class SphereLayer : public ZPG::Layer {
public:
    SphereLayer(ZPG::CameraController& cameraController);
    void OnAttach() override;
    void OnUpdate(ZPG::SceneContext& context) override;
private:
    ZPG::ref<ZPG::VertexArray> m_SphereVAO;
    ZPG::ref<ZPG::ShaderProgram> m_NormalShaderProgram;
    ZPG::CameraController& m_CameraController;
    ZPG::Timestep m_Timestep = 0;
};

