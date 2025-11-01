#pragma once 
#include "Transform/DynTransform.h"

namespace ZPG {

class Timestep;

class DynRotate : public DynamicTransform {
public:
    DynRotate(
        f32 startingRotationDeg, 
        f32 rotationSpeedDeg, 
        const v3& rotationAxis = v3(0.0, 1.0, 0.0));

    void Update(Timestep& ts) override;
    void ComputeMatrix() override;

    f32 GetCurrentRotationDeg() const;
    f32 GetRotationSpeedDeg() const;
    const v3& GetRotationAxis() const;

    void SetRotationSpeedDeg(f32 rotationSpeedDeg);
    void SetRotationAxis(const v3& rotationAxis);
    void SetRotationAxis(f32 x, f32 y, f32 z);

private:
    f32 m_CurrentRotationDeg;
    f32 m_RotationSpeedDeg;  // angle per second
    v3 m_RotationAxis;
};

}
