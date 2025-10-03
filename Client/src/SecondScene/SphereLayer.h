#pragma once

#include "ZPGine.h"

class SphereLayer : public ZPG::Layer {
public:
    SphereLayer(ZPG::CameraController& cameraController);
    void OnAttach() override;
    void OnUpdate(ZPG::SceneContext& context) override;
private:
    ZPG::Ref<ZPG::VertexArray> m_SphereVAO;
    ZPG::Ref<ZPG::ShaderProgram> m_NormalShaderProgram;
    ZPG::CameraController& m_CameraController;
    ZPG::Timestep m_Timestep = 0;
};

