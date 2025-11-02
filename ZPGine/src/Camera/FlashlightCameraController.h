//
// Created by phatt on 11/1/25.
//

#ifndef WORKSPACE_FLASHLIGHT_CAMERA_CONTROLLER_H
#define WORKSPACE_FLASHLIGHT_CAMERA_CONTROLLER_H
#include "CameraController.h"

namespace ZPG
{
class Light;

class SpotLight;

    class FlashlightCameraController : public CameraController
    {
    public:
        FlashlightCameraController(const ref<SpotLight>& light, Camera& camera);

        void OnEvent(Event& event) override;
        void OnUpdate(Timestep& ts) override;

    private:
        bool OnKeyPressed(KeyPressedEvent& event);

    private:
        bool m_FlashlightOn = true;
        ref<SpotLight> m_Light;
    };

} // ZPG

#endif