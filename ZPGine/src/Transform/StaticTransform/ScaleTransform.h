#pragma once
#include "Core/Core.h"
#include "Transform/StaticTransform.h"

namespace ZPG {

class ScaleTransform : public StaticTransform {
public:
    ScaleTransform(f32 scalar);
    ScaleTransform(f32 xScale, f32 yScale, f32 zScale);
    ScaleTransform(const glm::vec3& scale);
    void ComputeMatrix() override;
private:
    glm::vec3 m_ScaleVector;
};

}
