#pragma once
#include "Core/Core.h"
#include "Transform/StaticTransform.h"

namespace ZPG {

class Scale : public StaticTransform {
public:
    Scale(f32 scalar);
    Scale(f32 xScale, f32 yScale, f32 zScale);
    Scale(const glm::vec3& scale);
    void ComputeMatrix() override;
private:
    glm::vec3 m_ScaleVector;
};

}
