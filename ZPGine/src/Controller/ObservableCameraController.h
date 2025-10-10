#pragma once

#include "CameraController.h"
#include "Observer/Observable.h"

namespace ZPG {

class ObservableCameraController : public CameraController, public Observable {
public:
    ObservableCameraController(Camera& camera) : CameraController(camera) {
    }

    void OnUpdate(Timestep ts) override {
        CameraController::OnUpdate(ts);

        glm::vec3 pos = m_Camera.GetPosition();
        if (m_LastPosition != pos) {
            Payload payload(PayloadType::CameraPositionChanged, &pos);
            NotifyWith(payload);
            m_LastPosition = pos;
        }

        glm::vec3 front = m_Camera.GetFront();
        if (m_LastFront != front) {
            Payload payload(PayloadType::CameraOrientationChanged, &front);
            NotifyWith(payload);
            m_LastFront = front;
        }
    }
private:
    glm::vec3 m_LastPosition;
    glm::vec3 m_LastFront;
};

}
