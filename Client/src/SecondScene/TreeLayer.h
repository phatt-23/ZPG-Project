#pragma once

#include "ZPGine.h"

class TreeLayer : public ZPG::Layer {
public:
    TreeLayer(ZPG::CameraController& cameraController);
    void OnAttach() override;
    void OnUpdate(ZPG::Timestep ts) override;
private:
    ZPG::Ref<ZPG::VertexArray> m_TreeVAO;
    ZPG::Ref<ZPG::VertexArray> m_PlaneVao;
    ZPG::Ref<ZPG::ShaderProgram> m_NormalShaderProgram;
    ZPG::Ref<ZPG::ShaderProgram> m_RedShaderProgram;
    ZPG::Timestep m_Timestep = 0;
    ZPG::CameraController& m_CameraController;
};

