#pragma once

#include "ZPGine.h"

class ReadingFromFilesLayer : public ZPG::Layer {
public:
    ReadingFromFilesLayer(ZPG::CameraController& controller);
    void OnAttach() override;
    void OnUpdate([[maybe_unused]] ZPG::Timestep ts) override;
private: 
    ZPG::CameraController& m_CameraController;
    ZPG::Ref<ZPG::ShaderProgram> m_ShaderProgram;
    ZPG::Ref<ZPG::VertexArray> m_TreeVAO;
};
