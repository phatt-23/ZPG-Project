#pragma once

#include "ZPGine.h"

namespace CV8
{

    class ForestScene : public ZPG::Scene
    {
    public:
        ForestScene();

        void OnLazyAttach() override;
        void OnEvent(ZPG::Event& event) override;

        bool OnMouseButtonPressed(ZPG::MouseButtonPressedEvent& event);
    private:
        ZPG::ref<ZPG::ObservableCameraController> m_CameraController;
        ZPG::ResourceManager m_LocalRes;
    };

}
