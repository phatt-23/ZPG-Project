//
// Created by phatt on 11/1/25.
//

#include "FlashlightCameraController.h"

#include "Camera.h"
#include "Core/KeyCodes.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Light/SpotLight.h"

namespace ZPG {

FlashlightCameraController::FlashlightCameraController(const ref<SpotLight>& light, Camera& camera)
: CameraController(camera), m_Light(light)
{
}

void FlashlightCameraController::OnEvent(Event& event)
{
    CameraController::OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(FlashlightCameraController::OnKeyPressed));
}

void FlashlightCameraController::OnUpdate(Timestep& ts)
{
    CameraController::OnUpdate(ts);

    v3 camPos = GetCamera().GetPosition();
    v3 camDir = GetCamera().GetFront();

    m_Light->Position.Set(camPos);
    m_Light->Direction.Set(camDir);
}

bool FlashlightCameraController::OnKeyPressed(KeyPressedEvent& event)
{
    if (event.GetKeyCode() == ZPG_KEY_F) {
        m_FlashlightOn = !m_FlashlightOn;

        // this is dangerous, but whatever - every light has a color component
        SpotLight* light = ((SpotLight*)m_Light.get());
        v4 color = light->Color.Get();
        light->Color.Set(v4(v3(color), m_FlashlightOn ? 1.0f : 0.0f));
    }

    return false;
}

} // ZPG