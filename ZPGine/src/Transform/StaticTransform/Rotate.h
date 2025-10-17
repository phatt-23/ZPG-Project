#pragma once 
#include "Transform/StaticTransform.h"

namespace ZPG {

class Rotate : public StaticTransform {
public:
    Rotate(const v3& direction, const v3& up = v3(0.0, 1.0, 0.0));
    Rotate(const qtr& rotation);
    Rotate(f32 rotationAngleDeg, const v3& rotationAxis = v3(0.0, 1.0, 0.0));
    void ComputeMatrix() override;
private:
    qtr m_RotationQuaternion;
};

}
