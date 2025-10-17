#pragma once 
#include "Transform/DynTransform.h"

namespace ZPG {

class Timestep;

class DynRotate : public DynamicTransform {
public:
    DynRotate(
        f32 startingRotationDeg, 
        f32 rotationSpeedDeg, 
        const v3& rotationAxis);

    void Update(Timestep& ts) override;
    void ComputeMatrix() override;
private:
    f32 m_CurrentRotationDeg;
    f32 m_RotationSpeedDeg;  // angle per second
    v3 m_RotationAxis;
    v3 m_Origin;
};

}
