//
// Created by phatt on 10/18/25.
//
#include "ObservableCameraController.h"

#include "Observer/Payload.h"
#include "Camera.h"

namespace ZPG {

ObservableCameraController::ObservableCameraController(Camera& camera)
    : CameraController(camera)
    , m_LastPosition()
    , m_LastFront() {
}

void ObservableCameraController::OnUpdate(Timestep& ts) {
    CameraController::OnUpdate(ts);

    v3 pos = m_Camera.GetPosition();
    if (m_LastPosition != pos) {
        Payload payload(PayloadType::CameraPositionChanged, &pos);
        NotifyWith(payload);
        m_LastPosition = pos;
    }

    v3 front = m_Camera.GetFront();
    if (m_LastFront != front) {
        Payload payload(PayloadType::CameraOrientationChanged, &front);
        NotifyWith(payload);
        m_LastFront = front;
    }
}


}
