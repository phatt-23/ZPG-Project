#pragma once

#include "ZPGine.h"

namespace Obhajoba
{
    class ForestScene : public ZPG::Scene
    {
    public:
        ForestScene();

        void OnLazyAttach() override;
        void OnEvent(ZPG::Event& event) override;
        void OnRender() override;

        bool OnMouseButtonPressed(ZPG::MouseButtonPressedEvent& event);
    private:
        ZPG::ref<ZPG::ObservableCameraController> m_CameraController;
        ZPG::ResourceManager m_LocalRes;
        ZPG::EntityManager m_TransparentEntities;
        ZPG::EntityManager m_CustomShaderProgramEntities;
    };
}
