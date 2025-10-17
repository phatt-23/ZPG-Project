#pragma once

#include "CameraController.h"
#include "Observer/Observable.h"

namespace ZPG {

class ObservableCameraController : public CameraController, public Observable {
public:
    ObservableCameraController(Camera& camera);
    void OnUpdate(Timestep& ts) override;
private:
    v3 m_LastPosition;
    v3 m_LastFront;
};

}
